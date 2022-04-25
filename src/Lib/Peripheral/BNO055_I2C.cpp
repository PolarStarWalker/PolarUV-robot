#include "BNO055/BNO055_I2C.hpp"
#include "../Math/SIPrefix.hpp"

#include <iostream>

using namespace BNO055;

constexpr double ToRadians = std::numbers::pi_v<double> / 180;
constexpr double ToDegrees = 180 / std::numbers::pi_v<double>;

BNO055_I2C::BNO055_I2C(I2C& i2c, uint16_t sensorAddress, BNO055::OperationMode mode) :
        ISensor(i2c),
        sensorAddress_(sensorAddress),
        operationMode_(mode),
        filters_({new CircleMovingAverage<20>(),
                  new CircleMovingAverage<20>(),
                  new CircleMovingAverage<20>(),
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

SensorTask BNO055_I2C::Init() {
    for (;;) {

        co_await TimerType::SleepFor_ms(10);

        /// Connection test
        auto [id, isActive] = i2c_->ReadByteFromRegister(sensorAddress_, CHIP_ID_REG);
        if (id != BNO055::ID || !isActive)
            continue;

        /// Switch to config mode
        if (!i2c_->WriteByteToRegister(sensorAddress_, OPR_MODE_REG, OPERATION_MODE_CONFIG))
            continue;

        /// Reset
        if (!i2c_->WriteByteToRegister(sensorAddress_, SYS_TRIGGER_REG, 0x20))
            continue;

        co_await TimerType::SleepFor_ms(50);
        bool isContinue = false;
        for (size_t i = 0; i2c_->ReadByteFromRegister(sensorAddress_, CHIP_ID_REG).first != BNO055::ID; ++i) {
            if (i == 100) {
                isContinue = true;
                break;
            }

            co_await TimerType::SleepFor_ms(30);
        }

        if (isContinue)
            continue;

        co_await TimerType::SleepFor_ms(50);

        /// Set to normal power mode
        if (!i2c_->WriteByteToRegister(sensorAddress_, PWR_MODE_REG, POWER_MODE_NORMAL))
            continue;

        co_await TimerType::SleepFor_ms(10);
        if (!i2c_->WriteByteToRegister(sensorAddress_, PAGE_ID_REG, 0))
            continue;

        /// Set the requested operation mode
        if (!i2c_->WriteByteToRegister(sensorAddress_, SYS_TRIGGER_REG, 0x0))
            continue;

        co_await TimerType::SleepFor_ms(30);
        if (!i2c_->WriteByteToRegister(sensorAddress_, OPR_MODE_REG, operationMode_))
            continue;

        co_await TimerType::SleepFor_ms(100);

        co_yield true;
    }
}

SensorTask BNO055_I2C::ReadData() {
    for (;;) {

        co_await TimerType::SleepFor_us(2500);

        auto [calibrationData, isActive] = i2c_->ReadByteFromRegister(sensorAddress_, CALIB_STAT_REG);
        if (!isActive)
            continue;

        Data data{};
        data.CalibrationArray[0] = (uint8_t) ((calibrationData >> 6) & 0x03); // System
        data.CalibrationArray[1] = (uint8_t) ((calibrationData >> 4) & 0x03); // Gyroscope
        data.CalibrationArray[2] = (uint8_t) ((calibrationData >> 2) & 0x03); // Accelerometer
        data.CalibrationArray[3] = (uint8_t) (calibrationData & 0x03);        // Magnetometer

        co_await TimerType::SleepFor_us(2500);

        std::array<__u8, 6> buffer = {};
        if (!i2c_->ReadFromRegister(sensorAddress_, LINEAR_ACCEL_DATA_X_LSB_REG, buffer.begin(), buffer.size()))
            continue;

        auto accelerationX = (int16_t) (buffer[0] | (buffer[1] << 8));
        auto accelerationY = (int16_t) (buffer[2] | (buffer[3] << 8));
        auto accelerationZ = (int16_t) (buffer[4] | (buffer[5] << 8));

        data.LinearAcceleration[X] = ((double) accelerationX) / 100.0;
        data.LinearAcceleration[Y] = ((double) accelerationY) / 100.0;
        data.LinearAcceleration[Z] = ((double) accelerationZ) / 100.0;

        co_await TimerType::SleepFor_us(2500);

        if (!i2c_->ReadFromRegister(sensorAddress_, EULER_H_LSB_REG, buffer.begin(), buffer.size()))
            continue;

        auto eulerX = (int16_t) (buffer[0] | (buffer[1] << 8));
        auto eulerY = (int16_t) (buffer[2] | (buffer[3] << 8));
        auto eulerZ = (int16_t) (buffer[4] | (buffer[5] << 8));

        data.EulerAngle[X] = ((double) eulerZ) / 16.0;
        data.EulerAngle[Y] = -((double) eulerY) / 16.0;
        data.EulerAngle[Z] = ((double) eulerX) / 16.0;

        co_await TimerType::SleepFor_us(2500);

        auto [temperature, isActivee] = (i2c_->ReadByteFromRegister(sensorAddress_, TEMP_REG));

        if (!isActivee)
            continue;

        data.Temperature = temperature;

        data.EulerAngle[X] = (filters_[EulerAngleX](data.EulerAngle[X] * ToRadians)) * ToDegrees;
        data.EulerAngle[Y] = (filters_[EulerAngleY](data.EulerAngle[Y] * ToRadians)) * ToDegrees;
        data.EulerAngle[Z] = (filters_[EulerAngleZ](data.EulerAngle[Z] * ToRadians)) * ToDegrees;

        data.LinearAcceleration[X] = filters_[LinearAccelerationX](data.LinearAcceleration[X]);
        data.LinearAcceleration[Y] = filters_[LinearAccelerationY](data.LinearAcceleration[Y]);
        data.LinearAcceleration[Z] = filters_[LinearAccelerationZ](data.LinearAcceleration[Z]);

        std::cout << data.EulerAngle[X] <<'\n';

        SetData(data);

        co_yield true;
    }
}

Data BNO055_I2C::GetData() const {
    std::lock_guard guard(dataMutex_);
    Data data = data_;
    return data;
}
