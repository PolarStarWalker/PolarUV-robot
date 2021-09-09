#include "MS5837/MS5837.hpp"

MS5837::MS5837(const char *i2cDevice, uint16_t sensorAddress) : IPeripheral(IPeripheral::I2CBus, IPeripheral::Read) {
    this->_i2c = new I2C(i2cDevice);
    this->_address = sensorAddress;
}

bool MS5837::Reload() {
    std::lock_guard<std::mutex> mutex(this->_dataMutex);
    _i2c->WriteByte(MS5837_ADDRESS, 0x00, MS5837_RESET);
    return true;
}

bool MS5837::Initialize() {

    this->Reload();
    usleep(10 * 1000);

    // Read calibration values and CRC
    for (uint8_t i = 0; i < 7; i++) {
        _i2c->WriteByte(MS5837_ADDRESS, 0x00, MS5837_PROM_READ + i * 2);
        C[i] = (_i2c->ReadByte(MS5837_ADDRESS, 0x00) << 8) | _i2c->ReadByte(MS5837_ADDRESS, 0x01);
    }

    // Verify that data is correct with CRC
    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = CRC4(C);

    if (crcCalculated != crcRead) {
        return false; // CRC fail
    }

    uint8_t version = (C[0] >> 5) & 0x7F; // Extract the sensor version from PROM Word 0

    // Set _model according to the sensor version
    if (version == MS5837_02BA01) {
        _model = MS5837_02BA;
    } else if (version == MS5837_02BA21) {
        _model = MS5837_02BA;
    } else if (version == MS5837_30BA26) {
        _model = MS5837_30BA;
    } else {
        _model = MS5837_UNRECOGNISED;
    }

    this->_status = true;
    return true;
}

bool MS5837::ReadData() {
    // Mutex is not needed, since this function is called in GetData()
    //std::lock_guard<std::mutex> mutex(this->_dataMutex);

    // Request D1 conversion
    _i2c->WriteByte(MS5837_ADDRESS, 0x00, MS5837_CONVERT_D1_8192);

    usleep(20 * 1000); // Max conversion time per datasheet

    _i2c->WriteByte(MS5837_ADDRESS, 0x00, MS5837_ADC_READ);

    _d1Pressure = _i2c->ReadByte(MS5837_ADDRESS, 0x00);
    _d1Pressure = (_d1Pressure << 8) | _i2c->ReadByte(MS5837_ADDRESS, 0x01);
    _d1Pressure = (_d1Pressure << 8) | _i2c->ReadByte(MS5837_ADDRESS, 0x02);

    // Request D2 conversion
    _i2c->WriteByte(MS5837_ADDRESS, 0x00, MS5837_CONVERT_D2_8192);

    usleep(20 * 1000); // Max conversion time per datasheet

    _i2c->WriteByte(MS5837_ADDRESS, 0x00, MS5837_ADC_READ);

    _d2Temperature = _i2c->ReadByte(MS5837_ADDRESS, 0x00);
    _d2Temperature = (_d2Temperature << 8) | _i2c->ReadByte(MS5837_ADDRESS, 0x01);
    _d2Temperature = (_d2Temperature << 8) | _i2c->ReadByte(MS5837_ADDRESS, 0x02);

    Calculate();

    double pressure = 0.0;
    (_model == MS5837_02BA) ?
            pressure = (_p * 0.001 / 100.0) :
            pressure = (_p * 0.001 / 10.0);
    this->_data.pressure = pressure;

    this->_data.temperature = (double) _temperature / 100.0;

    double depth = 0.0;
    (_model == MS5837_02BA) ?
            depth = (_p - 101300) / (_fluidDensity * 9.80665) :
            depth = ((_p * 10.0) - 101300) / (_fluidDensity * 9.80665);
    this->_data.depth = depth;

    return true;
}

bool MS5837::WriteData() {
    return false;
}

void MS5837::SetFluidDensity(double density) {
    _fluidDensity = density;
}

void MS5837::Calculate() {

    int32_t dT = 0;
    int64_t SENS = 0;
    int64_t OFF = 0;
    int32_t SENSi = 0;
    int32_t OFFi = 0;
    int32_t Ti = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    // Terms called
    dT = _d2Temperature - uint32_t(C[5]) * 256l;
    if (_model == MS5837_02BA) {
        SENS = int64_t(C[1]) * 65536l + (int64_t(C[3]) * dT) / 128l;
        OFF = int64_t(C[2]) * 131072l + (int64_t(C[4]) * dT) / 64l;
        _p = (_d1Pressure * SENS / (2097152l) - OFF) / (32768l);
    } else {
        SENS = int64_t(C[1]) * 32768l + (int64_t(C[3]) * dT) / 256l;
        OFF = int64_t(C[2]) * 65536l + (int64_t(C[4]) * dT) / 128l;
        _p = (_d1Pressure * SENS / (2097152l) - OFF) / (8192l);
    }

    // Temp conversion
    _temperature = 2000l + int64_t(dT) * C[6] / 8388608LL;

    //Second order compensation
    if (_model == MS5837_02BA) {
        if ((_temperature / 100) < 20) {         //Low temp
            Ti = (11 * int64_t(dT) * int64_t(dT)) / (34359738368LL);
            OFFi = (31 * (_temperature - 2000) * (_temperature - 2000)) / 8;
            SENSi = (63 * (_temperature - 2000) * (_temperature - 2000)) / 32;
        }
    } else {
        if ((_temperature / 100) < 20) {         //Low temp
            Ti = (3 * int64_t(dT) * int64_t(dT)) / (8589934592LL);
            OFFi = (3 * (_temperature - 2000) * (_temperature - 2000)) / 2;
            SENSi = (5 * (_temperature - 2000) * (_temperature - 2000)) / 8;
            if ((_temperature / 100) < -15) {    //Very low temp
                OFFi = OFFi + 7 * (_temperature + 1500l) * (_temperature + 1500l);
                SENSi = SENSi + 4 * (_temperature + 1500l) * (_temperature + 1500l);
            }
        } else if ((_temperature / 100) >= 20) {    //High temp
            Ti = 2 * (dT * dT) / (137438953472LL);
            OFFi = (1 * (_temperature - 2000) * (_temperature - 2000)) / 16;
            SENSi = 0;
        }
    }

    OFF2 = OFF - OFFi; //Calculate pressure and temp second order
    SENS2 = SENS - SENSi;

    _temperature = (_temperature - Ti);

    if (_model == MS5837_02BA) {
        _p = (((_d1Pressure * SENS2) / 2097152l - OFF2) / 32768l);
    } else {
        _p = (((_d1Pressure * SENS2) / 2097152l - OFF2) / 8192l);
    }
}

uint8_t MS5837::CRC4(uint16_t *n_prom) {
    uint16_t n_rem = 0;

    n_prom[0] = ((n_prom[0]) & 0x0FFF);
    n_prom[7] = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if (i % 2 == 1) {
            n_rem ^= (uint16_t) ((n_prom[i >> 1]) & 0x00FF);
        } else {
            n_rem ^= (uint16_t) (n_prom[i >> 1] >> 8);
        }
        for (uint8_t n_bit = 8; n_bit > 0; n_bit--) {
            if (n_rem & 0x8000) {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else {
                n_rem = (n_rem << 1);
            }
        }
    }

    n_rem = ((n_rem >> 12) & 0x000F);
    return n_rem ^ 0x00;
}

MS5837Data MS5837::GetData() {
    std::lock_guard<std::mutex> mutex(this->_dataMutex);

    this->ReadData();

    return this->_data;
}

