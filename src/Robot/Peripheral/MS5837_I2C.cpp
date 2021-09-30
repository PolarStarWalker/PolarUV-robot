#include "./MS5837/MS5837_I2C.hpp"

using namespace MS5837;

MS5837_I2C::MS5837_I2C(uint16_t sensorAddress) {
    this->_sensorAddress = sensorAddress;
}

MS5837_I2C::~MS5837_I2C() {
    delete this->_i2c;
}

bool MS5837_I2C::Init(const I2C *i2c) {

    this->_i2c = i2c;

    this->Reload();
    usleep(10 * 1000);

    for (uint8_t i = 0; i < 7; i++) {
        _i2c->WriteByte(this->_sensorAddress, MS5837_PROM_READ + (i * 2));
        uint8_t cData[2]{};
        _i2c->Read(this->_sensorAddress, cData, 2);
        C[i] = (cData[0] << 8) | cData[1];
    }

    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = CRC4(C);

    return crcCalculated == crcRead;
}

bool MS5837_I2C::ReadData() {

    _i2c->WriteByte(this->_sensorAddress, MS5837_CONVERT_D1_8192);

    usleep(20 * 1000); // Max conversion time per datasheet

    _i2c->WriteByte(this->_sensorAddress, MS5837_ADC_READ);

    uint8_t d1Data[3]{};
    _i2c->Read(this->_sensorAddress, d1Data, 3);
    _d1Pressure = d1Data[0];
    _d1Pressure = (_d1Pressure << 8) | d1Data[1];
    _d1Pressure = (_d1Pressure << 8) | d1Data[2];

    _i2c->WriteByte(this->_sensorAddress, MS5837_CONVERT_D2_8192);

    usleep(20 * 1000);

    _i2c->WriteByte(this->_sensorAddress, MS5837_ADC_READ);

    uint8_t d2Data[3]{};
    _i2c->Read(this->_sensorAddress, d2Data, 3);
    _d2Temperature = d2Data[0];
    _d2Temperature = (_d2Temperature << 8) | d2Data[1];
    _d2Temperature = (_d2Temperature << 8) | d2Data[2];

    Calculate();

    MS5837::Data data{};

    double pressure = _p * 0.001 / 10.0;
    data.Pressure = pressure;

    data.Temperature = (double) _temperature / 100.0;

    double depth = ((_p * 10.0) - 101300) / (_fluidDensity * 9.80665);
    data.Depth = depth;

    this->SetData(data);

    return true;
}

void MS5837_I2C::SetFluidDensity(double density) {
    _fluidDensity = density;
}

void MS5837_I2C::Calculate() {

    int32_t dT = 0;
    int64_t SENS = 0;
    int64_t OFF = 0;
    int32_t SENSi = 0;
    int32_t OFFi = 0;
    int32_t Ti = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    dT = _d2Temperature - uint32_t(C[5]) * 256l;
    SENS = int64_t(C[1]) * 32768l + (int64_t(C[3]) * dT) / 256l;
    OFF = int64_t(C[2]) * 65536l + (int64_t(C[4]) * dT) / 128l;
    _p = (_d1Pressure * SENS / (2097152l) - OFF) / (8192l);

    _temperature = 2000l + int64_t(dT) * C[6] / 8388608LL;

    if ((_temperature / 100) < 20) {
        Ti = (3 * int64_t(dT) * int64_t(dT)) / (8589934592LL);
        OFFi = (3 * (_temperature - 2000) * (_temperature - 2000)) / 2;
        SENSi = (5 * (_temperature - 2000) * (_temperature - 2000)) / 8;
        if ((_temperature / 100) < -15) {
            OFFi = OFFi + 7 * (_temperature + 1500l) * (_temperature + 1500l);
            SENSi = SENSi + 4 * (_temperature + 1500l) * (_temperature + 1500l);
        }
    } else if ((_temperature / 100) >= 20) {
        Ti = 2 * (dT * dT) / (137438953472LL);
        OFFi = (1 * (_temperature - 2000) * (_temperature - 2000)) / 16;
        SENSi = 0;
    }

    OFF2 = OFF - OFFi;
    SENS2 = SENS - SENSi;

    _temperature = (_temperature - Ti);
    _p = (((_d1Pressure * SENS2) / 2097152l - OFF2) / 8192l);
}

uint8_t MS5837_I2C::CRC4(uint16_t *n_prom) {
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

MS5837::Data MS5837_I2C::GetData() const{
    this->_dataMutex.lock_shared();
    MS5837::Data data = this->_data;
    this->_dataMutex.unlock_shared();

    return data;
}

bool MS5837_I2C::Reload() {
    _i2c->WriteByte(this->_sensorAddress, MS5837_RESET);
    return true;
}

