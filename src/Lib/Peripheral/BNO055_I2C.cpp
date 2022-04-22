#include "BNO055/BNO055_I2C.hpp"
#include "../Math/SIPrefix.hpp"

#include <iostream>

using namespace BNO055;

constexpr double ToRadians = std::numbers::pi_v<double> / 180;
constexpr double ToDegrees = 180 / std::numbers::pi_v<double>;

BNO055_I2C::BNO055_I2C(uint16_t sensorAddress, BNO055::OperationMode mode) :
        ISensor(Kilo(10)),
        ReadDataAsync(ReadDataImpl()),
        InitSensorsAsync(InitSensorsImpl()),
        sensorAddress_(sensorAddress),
        operationMode_(mode),
        filters_({new CircleMovingAverage<10>(),
                  new CircleMovingAverage<10>(),
                  new CircleMovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>(),
                  new MovingAverage<10>()}) {}

bool BNO055_I2C::Init(const I2C *i2c, TimerType &timer) {
    i2c_ = i2c;
    auto [time, isActive] = InitSensorsAsync();
    timer.SetTimer(time);
    return isActive;
}

bool BNO055_I2C::ReadData(TimerType &timer) {
    auto [time, isActive] = ReadDataAsync();
    timer.SetTimer(time);
    return isActive;
}

Data BNO055_I2C::GetData() const {
    std::lock_guard guard(dataMutex_);
    Data data = data_;
    return data;
}

SensorTask BNO055_I2C::ReadDataImpl() {
    for (;;) {

        co_await TimerType::SleepFor_ms(20);

        auto [calibrationData, isActive] = i2c_->ReadByteFromRegister(sensorAddress_, CALIB_STAT_REG);
        if (!isActive) {
            co_yield false;
            continue;
        }

        Data data{};
        data.CalibrationArray[0] = (uint8_t) ((calibrationData >> 6) & 0x03); // System
        data.CalibrationArray[1] = (uint8_t) ((calibrationData >> 4) & 0x03); // Gyroscope
        data.CalibrationArray[2] = (uint8_t) ((calibrationData >> 2) & 0x03); // Accelerometer
        data.CalibrationArray[3] = (uint8_t) (calibrationData & 0x03);        // Magnetometer

        std::array<__u8, 12> buffer = {};
        __u8 accelerationRegister = LINEAR_ACCEL_DATA_X_LSB_REG;
        if (!i2c_->Read(sensorAddress_, &accelerationRegister, 1, buffer.begin(), buffer.size())) {
            co_yield false;
            continue;
        }

        auto accelerationX = (int16_t) (buffer[0] | (buffer[1] << 8));
        auto accelerationY = (int16_t) (buffer[2] | (buffer[3] << 8));
        auto accelerationZ = (int16_t) (buffer[4] | (buffer[5] << 8));

        data.LinearAcceleration[X] = ((double) accelerationX) / 100.0;
        data.LinearAcceleration[Y] = ((double) accelerationY) / 100.0;
        data.LinearAcceleration[Z] = ((double) accelerationZ) / 100.0;

        __u8 eulerRegister = EULER_H_LSB_REG;
        if (!i2c_->Read(sensorAddress_, &eulerRegister, 1, buffer.begin(), buffer.size())) {
            co_yield false;
            continue;
        }

        auto eulerX = (int16_t) (buffer[0] | (buffer[1] << 8));
        auto eulerY = (int16_t) (buffer[2] | (buffer[3] << 8));
        auto eulerZ = (int16_t) (buffer[4] | (buffer[5] << 8));

        data.EulerAngle[X] = ((double) eulerZ) / 16.0;
        data.EulerAngle[Y] = -((double) eulerY) / 16.0;
        data.EulerAngle[Z] = ((double) eulerX) / 16.0;

        auto [temperature, isActivee] = (i2c_->ReadByteFromRegister(sensorAddress_, TEMP_REG));

        if (!isActivee) {
            co_yield false;
            continue;
        }

        data.Temperature = temperature;

        data.EulerAngle[X] = (filters_[EulerAngleX](data.EulerAngle[X] * ToRadians)) * ToDegrees;
        data.EulerAngle[Y] = (filters_[EulerAngleY](data.EulerAngle[Y] * ToRadians)) * ToDegrees;
        data.EulerAngle[Z] = (filters_[EulerAngleZ](data.EulerAngle[Z] * ToRadians)) * ToDegrees;

        data.LinearAcceleration[X] = filters_[LinearAccelerationX](data.LinearAcceleration[X]);
        data.LinearAcceleration[Y] = filters_[LinearAccelerationY](data.LinearAcceleration[Y]);
        data.LinearAcceleration[Z] = filters_[LinearAccelerationZ](data.LinearAcceleration[Z]);

        SetData(data);

        co_yield true;
    }
}

SensorTask BNO055_I2C::InitSensorsImpl() {
    for (;;) {

        co_await TimerType::SleepFor_ms(10);
        co_yield false;

        /// Connection test
        auto [id, isActive] = i2c_->ReadByteFromRegister(sensorAddress_, CHIP_ID_REG);
        if (id != BNO055_ID || !isActive) {
            co_yield false;
            continue;
        };

        /// Switch to config mode
        if (!i2c_->WriteByteToRegister(sensorAddress_, OPR_MODE_REG, OPERATION_MODE_CONFIG)) {
            co_yield false;
            continue;
        }

        /// Reset
        if (!i2c_->WriteByteToRegister(sensorAddress_, SYS_TRIGGER_REG, 0x20)) {
            co_yield false;
            continue;
        }
        co_await TimerType::SleepFor_ms(50);
        bool isContinue = false;
        for (size_t i = 0; i2c_->ReadByteFromRegister(sensorAddress_, CHIP_ID_REG).first != BNO055_ID; ++i) {
            if (i == 100) {
                isContinue = true;
                break;
            }

            co_await TimerType::SleepFor_ms(30);
        }
        if (isContinue) {
            co_yield false;
            continue;
        }
        co_await TimerType::SleepFor_ms(50);

        /// Set to normal power mode
        if (!i2c_->WriteByteToRegister(sensorAddress_, PWR_MODE_REG, POWER_MODE_NORMAL)) {
            co_yield false;
            continue;
        }
        co_await TimerType::SleepFor_ms(10);
        if (!i2c_->WriteByteToRegister(sensorAddress_, PAGE_ID_REG, 0)) {
            co_yield false;
            continue;
        }


        /// Set the requested operation mode
        if (!i2c_->WriteByteToRegister(sensorAddress_, SYS_TRIGGER_REG, 0x0)) {
            co_yield false;
            continue;
        }
        co_await TimerType::SleepFor_ms(30);
        if (!i2c_->WriteByteToRegister(sensorAddress_, OPR_MODE_REG, operationMode_)) {
            co_yield false;
            continue;
        }

        co_await TimerType::SleepFor_ms(100);

        co_yield true;
    }
}
