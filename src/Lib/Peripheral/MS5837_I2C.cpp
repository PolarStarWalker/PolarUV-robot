#include "./MS5837/MS5837_I2C.hpp"
#include "../Math/SIPrefix.hpp"

#include <iostream>

using namespace MS5837;

MS5837_I2C::MS5837_I2C(I2C& i2c,uint16_t sensorAddress) :
        ISensor(i2c),
        sensorCalibration_({}),
        filters_({new MovingAverage<5>(), new MovingAverage<5>(), new MovingAverage<5>()}),
        sensorAddress_(sensorAddress){}

SensorTask MS5837_I2C::Init() {
    for (;;) {

        co_await TimerType::SleepFor_ms(10);

        if (!i2c_->WriteByte(sensorAddress_, MS5837_RESET))
            continue;

        co_await TimerType::SleepFor_ms(10);

        int isContinue = false;
        for (uint8_t i = 0; i < 7; i++) {
            i2c_->WriteByte(sensorAddress_, MS5837_PROM_READ + (i * 2));
            uint8_t cData[2]{};
            i2c_->Read(sensorAddress_, cData, 2);
            if (cData[0] == 0 && cData[1] == 0) {
                isContinue = true;
                break;
            }
            sensorCalibration_[i] = (cData[0] << 8) | cData[1];
        }

        if (isContinue)
            continue;

        uint8_t crcRead = sensorCalibration_[0] >> 12;
        uint8_t crcCalculated = CRC4(sensorCalibration_);

        if (crcCalculated != crcRead)
            continue;

        co_yield true;
    }
}

SensorTask MS5837_I2C::ReadData() {
    for(;;){
        for (;;) {

            co_await TimerType::SleepFor_ms(10);

            if (!i2c_->WriteByte(sensorAddress_, MS5837_CONVERT_D1_8192))
                break;

            co_await TimerType::SleepFor_ms(20); // Max conversation time per datasheet

            if (!i2c_->WriteByte(sensorAddress_, MS5837_ADC_READ))
                break;

            std::array<uint8_t, 3> d1Data{};
            if (!i2c_->Read(sensorAddress_, d1Data.begin(), d1Data.size()))
                break;

            MS5837::Measure measure;
            measure.d1Pressure_ = d1Data[0];
            measure.d1Pressure_ = (measure.d1Pressure_ << 8) | d1Data[1];
            measure.d1Pressure_ = (measure.d1Pressure_ << 8) | d1Data[2];

            if (!i2c_->WriteByte(sensorAddress_, MS5837_CONVERT_D2_8192))
                break;

            co_await TimerType::SleepFor_ms(20);

            if (!i2c_->WriteByte(sensorAddress_, MS5837_ADC_READ))
                break;

            std::array<uint8_t, 3> d2Data{};
            if (!i2c_->Read(sensorAddress_, d2Data.begin(), d2Data.size()))
                break;

            measure.D2Temperature = d2Data[0];
            measure.D2Temperature = (measure.D2Temperature << 8) | d2Data[1];
            measure.D2Temperature = (measure.D2Temperature << 8) | d2Data[2];

            measure.FluidDensity = fluidDensity_;

            MS5837::Data data = Calculate(measure, sensorCalibration_);

            data.Depth = filters_[Depth](data.Depth);
            data.Pressure = filters_[Pressure](data.Pressure);
            data.Temperature = filters_[Temperature](data.Temperature);

            SetData(data);
        }

        co_yield false;
    }
}

void MS5837_I2C::SetFluidDensity(double density) {
    fluidDensity_ = density;
}

MS5837::Data MS5837_I2C::Calculate(const MS5837::Measure &measure, const std::array<uint8_t, 8> &Calibration) {

    int64_t SENS = 0;
    int64_t OFF = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;
    int32_t SENSi = 0;
    int32_t OFFi = 0;
    int32_t Ti = 0;
    int32_t dT = 0;
    int32_t pressure = 0;
    int32_t temperature = 0;

    dT = measure.D2Temperature - uint32_t(Calibration[5]) * 256l;
    SENS = int64_t(Calibration[1]) * 32768l + (int64_t(Calibration[3]) * dT) / 256l;
    OFF = int64_t(Calibration[2]) * 65536l + (int64_t(Calibration[4]) * dT) / 128l;

    temperature = 2000l + int64_t(dT) * Calibration[6] / 8388608LL;

    if ((temperature / 100) < 20) {
        Ti = (3 * int64_t(dT) * int64_t(dT)) / (8589934592LL);
        OFFi = (3 * (temperature - 2000) * (temperature - 2000)) / 2;
        SENSi = (5 * (temperature - 2000) * (temperature - 2000)) / 8;
        if ((temperature / 100) < -15) {
            OFFi = OFFi + 7 * (temperature + 1500l) * (temperature + 1500l);
            SENSi = SENSi + 4 * (temperature + 1500l) * (temperature + 1500l);
        }
    } else {
        Ti = 2 * (dT * dT) / (137438953472LL);
        OFFi = (temperature - 2000) * (temperature - 2000) / 16;
        SENSi = 0;
    }

    OFF2 = OFF - OFFi;
    SENS2 = SENS - SENSi;

    pressure = (measure.d1Pressure_ * SENS2 / (2097152l) - OFF2) / (8192l);

    MS5837::Data data{};

    data.Pressure = pressure * 0.001 / 10.0;
    data.Depth = ((pressure * 10.0) - 101300) / (measure.FluidDensity * 9.80665);
    data.Temperature = (temperature - Ti) / 100.0;

    return data;
}

uint8_t MS5837_I2C::CRC4(std::array<uint8_t, 8> &n_prom) {
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
    std::lock_guard guard(dataMutex_);
    MS5837::Data data = data_;
    return data;
}