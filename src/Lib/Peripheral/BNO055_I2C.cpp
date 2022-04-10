#include "BNO055/BNO055_I2C.hpp"
#include "../Math/SIPrefix.hpp"

using namespace BNO055;

constexpr double ToRadians = M_PIf64 / 180;

constexpr double ToDegrees = 180 / M_PIf64;

BNO055_I2C::BNO055_I2C(uint16_t sensorAddress, BNO055::OperationMode mode) :
        II2CPeripheral(Kilo(10)),
        _filters({new CircleMovingAverage<10>(),
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
                  new MovingAverage<10>()}) {
    _sensorAddress = sensorAddress;
    _operationMode = mode;
}

bool BNO055_I2C::Init(const I2C *i2c) {

    this->_i2c = i2c;

    /// Connection test
    uint8_t id = _i2c->ReadByteFromRegister(_sensorAddress, CHIP_ID_REG);
    if (id != BNO055_ID) {
        usleep(Mega(1));
        id = _i2c->ReadByteFromRegister(_sensorAddress, CHIP_ID_REG);
        if (id != BNO055_ID) {
            return false;
        }
    }

    /// Switch to config mode
    SendOperationMode(OPERATION_MODE_CONFIG);

    /// Reset
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x20);
    usleep(Kilo(30));
    while (_i2c->ReadByteFromRegister(_sensorAddress, CHIP_ID_REG) != BNO055_ID) {
        usleep(Kilo(30));
    }
    usleep(Kilo(50));

    /// Set to normal power mode
    _i2c->WriteByteToRegister(_sensorAddress, PWR_MODE_REG, POWER_MODE_NORMAL);
    usleep(Kilo(10));
    _i2c->WriteByteToRegister(_sensorAddress, PAGE_ID_REG, 0);

    /// Set the requested operation mode
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x0);
    usleep(Kilo(30));
    SendOperationMode(this->_operationMode);
    usleep(Kilo(20));

    return true;
}

void BNO055_I2C::SendOperationMode(OperationMode mode) const {
    _i2c->WriteByteToRegister(_sensorAddress, OPR_MODE_REG, mode);
    usleep(Kilo(20));
}

void BNO055_I2C::UseExternalCrystal(bool useExtCrl) {
    OperationMode previousMode = _operationMode;

    SendOperationMode(OPERATION_MODE_CONFIG);
    usleep(25 * 1000);

    _i2c->WriteByteToRegister(_sensorAddress, PAGE_ID_REG, 0);

    useExtCrl ?
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x80) :
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x00);

    usleep(10 * 1000);

    SendOperationMode(previousMode);
    usleep(20 * 1000);
}

bool BNO055_I2C::ReadData() {

    Data data{};

    uint8_t calibrationData = _i2c->ReadByteFromRegister(_sensorAddress, CALIB_STAT_REG);
    data.CalibrationArray[0] = (uint8_t) ((calibrationData >> 6) & 0x03); // System
    data.CalibrationArray[1] = (uint8_t) ((calibrationData >> 4) & 0x03); // Gyroscope
    data.CalibrationArray[2] = (uint8_t) ((calibrationData >> 2) & 0x03); // Accelerometer
    data.CalibrationArray[3] = (uint8_t) (calibrationData & 0x03);        // Magnetometer

    __u8 accelerationBuffer[6] = {};
    __u8 accelerationRegister = LINEAR_ACCEL_DATA_X_LSB_REG;
    _i2c->Read(_sensorAddress, &accelerationRegister, 1, accelerationBuffer, 6);

    auto accelerationX = (int16_t) (accelerationBuffer[0] | (accelerationBuffer[1] << 8));
    auto accelerationY = (int16_t) (accelerationBuffer[2] | (accelerationBuffer[3] << 8));
    auto accelerationZ = (int16_t) (accelerationBuffer[4] | (accelerationBuffer[5] << 8));

    data.LinearAcceleration[X] = ((double) accelerationX) / 100.0;
    data.LinearAcceleration[Y] = ((double) accelerationY) / 100.0;
    data.LinearAcceleration[Z] = ((double) accelerationZ) / 100.0;

    __u8 eulerBuffer[6] = {};
    __u8 eulerRegister = EULER_H_LSB_REG;
    _i2c->Read(_sensorAddress, &eulerRegister, 1, eulerBuffer, 6);

    auto eulerX = (int16_t) (eulerBuffer[0] | (eulerBuffer[1] << 8));
    auto eulerY = (int16_t) (eulerBuffer[2] | (eulerBuffer[3] << 8));
    auto eulerZ = (int16_t) (eulerBuffer[4] | (eulerBuffer[5] << 8));

    data.EulerAngle[X] = ((double) eulerZ) / 16.0;
    data.EulerAngle[Y] = -((double) eulerY) / 16.0;
    data.EulerAngle[Z] = ((double) eulerX) / 16.0;

    data.Temperature = (int8_t) (_i2c->ReadByteFromRegister(_sensorAddress, TEMP_REG));

    data.EulerAngle[X] = (_filters[EulerAngleX](data.EulerAngle[X] * ToRadians)) * ToDegrees;
    data.EulerAngle[Y] = (_filters[EulerAngleY](data.EulerAngle[Y] * ToRadians)) * ToDegrees;
    data.EulerAngle[Z] = (_filters[EulerAngleZ](data.EulerAngle[Z] * ToRadians)) * ToDegrees;

    data.LinearAcceleration[X] = _filters[LinearAccelerationX](data.LinearAcceleration[X]);
    data.LinearAcceleration[Y] = _filters[LinearAccelerationY](data.LinearAcceleration[Y]);
    data.LinearAcceleration[Z] = _filters[LinearAccelerationZ](data.LinearAcceleration[Z]);

    SetData(data);

    return true;
}

Data BNO055_I2C::GetData() const {
    this->_dataMutex.lock_shared();
    Data data = this->_data;
    this->_dataMutex.unlock_shared();
    return data;
}

bool BNO055_I2C::Reload() {
    return false;
}

size_t BNO055_I2C::DelayUs() const {
    return Kilo(10);
}
