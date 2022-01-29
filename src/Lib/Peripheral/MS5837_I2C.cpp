#include "./MS5837/MS5837_I2C.hpp"
#include "../Math/SIPrefix.hpp"

using namespace MS5837;

MS5837_I2C::MS5837_I2C(uint16_t sensorAddress) :
        II2CPeripheral(Kilo(20)),
        sensorAddress_(sensorAddress),
        filters_({new MovingAverage<5>(), new MovingAverage<5>(), new MovingAverage<5>()}){}

MS5837_I2C::~MS5837_I2C() {
    delete i2c_;
}

bool MS5837_I2C::Init(const I2C *i2c) {

    i2c_ = i2c;

    Reload();
    usleep(10 * 1000);

    for (uint8_t i = 0; i < 7; i++) {
        i2c_->WriteByte(this->sensorAddress_, MS5837_PROM_READ + (i * 2));
        uint8_t cData[2]{};
        i2c_->Read(this->sensorAddress_, cData, 2);
        if (cData[0] == 0 && cData[1] == 0)
            return false;
        C_[i] = (cData[0] << 8) | cData[1];
    }

    uint8_t crcRead = C_[0] >> 12;
    uint8_t crcCalculated = CRC4(C_);

    return crcCalculated == crcRead;
}

bool MS5837_I2C::ReadData() {

    i2c_->WriteByte(sensorAddress_, MS5837_CONVERT_D1_8192);

    usleep(20 * 1000); // Max conversion time per datasheet

    i2c_->WriteByte(sensorAddress_, MS5837_ADC_READ);

    uint8_t d1Data[3]{};
    i2c_->Read(sensorAddress_, d1Data, 3);
    d1Pressure_ = d1Data[0];
    d1Pressure_ = (d1Pressure_ << 8) | d1Data[1];
    d1Pressure_ = (d1Pressure_ << 8) | d1Data[2];

    i2c_->WriteByte(sensorAddress_, MS5837_CONVERT_D2_8192);

    usleep(20 * 1000);

    i2c_->WriteByte(sensorAddress_, MS5837_ADC_READ);

    uint8_t d2Data[3]{};
    i2c_->Read(this->sensorAddress_, d2Data, 3);
    d2Temperature_ = d2Data[0];
    d2Temperature_ = (d2Temperature_ << 8) | d2Data[1];
    d2Temperature_ = (d2Temperature_ << 8) | d2Data[2];

    Calculate();

    MS5837::Data data{};

    double pressure = _p * 0.001 / 10.0;
    data.Pressure = filters_[Pressure](pressure);

    data.Temperature = filters_[Temperature]((double) _temperature / 100.0);

    double depth = ((_p * 10.0) - 101300) / (fluidDensity_ * 9.80665);
    data.Depth = filters_[Depth](depth);

    SetData(data);

    return true;
}

void MS5837_I2C::SetFluidDensity(double density) {
    fluidDensity_ = density;
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

    dT = d2Temperature_ - uint32_t(C_[5]) * 256l;
    SENS = int64_t(C_[1]) * 32768l + (int64_t(C_[3]) * dT) / 256l;
    OFF = int64_t(C_[2]) * 65536l + (int64_t(C_[4]) * dT) / 128l;
    _p = (d1Pressure_ * SENS / (2097152l) - OFF) / (8192l);

    _temperature = 2000l + int64_t(dT) * C_[6] / 8388608LL;

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
    _p = (((d1Pressure_ * SENS2) / 2097152l - OFF2) / 8192l);
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

MS5837::Data MS5837_I2C::GetData() const {
    this->dataMutex_.lock_shared();
    MS5837::Data data = this->data_;
    this->dataMutex_.unlock_shared();

    return data;
}

bool MS5837_I2C::Reload() {
    i2c_->WriteByte(this->sensorAddress_, MS5837_RESET);
    return true;
}

size_t MS5837_I2C::DelayUs() const {
    return Kilo(20);
}

