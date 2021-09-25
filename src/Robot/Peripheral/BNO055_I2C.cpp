#include "BNO055/BNO055_I2C.hpp"

using namespace BNO055;

BNO055_I2C::BNO055_I2C(uint16_t sensorAddress, BNO055::OperationMode mode) {
    _sensorAddress = sensorAddress;
    _operationMode = mode;
}

bool BNO055_I2C::Init(const I2C *i2c) {

    this->_i2c = i2c;

    /// Connection test
    uint8_t id = _i2c->ReadByteFromRegister(_sensorAddress, CHIP_ID_REG);
    if (id != BNO055_ID) {
        usleep(1000 * 1000);
        id = _i2c->ReadByteFromRegister(_sensorAddress, CHIP_ID_REG);
        if (id != BNO055_ID) {
            return false;
        }
    }

    /// Switch to config mode
    SendOperationMode(OPERATION_MODE_CONFIG);

    /// Reset
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x20);
    usleep(30 * 1000);
    while (_i2c->ReadByteFromRegister(_sensorAddress, CHIP_ID_REG) != BNO055_ID) {
        usleep(10 * 1000);
    }
    usleep(50 * 1000);

    /// Set to normal power mode
    _i2c->WriteByteToRegister(_sensorAddress, PWR_MODE_REG, POWER_MODE_NORMAL);
    usleep(10 * 1000);
    _i2c->WriteByteToRegister(_sensorAddress, PAGE_ID_REG, 0);

    /// Set the requested operation mode
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x0);
    usleep(10 * 1000);
    SendOperationMode(this->_operationMode);
    usleep(20 * 1000);

    return true;
}

void BNO055_I2C::SendOperationMode(OperationMode mode) {
    _i2c->WriteByteToRegister(_sensorAddress, OPR_MODE_REG, mode);
    usleep(30 * 1000);
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
    int16_t accelerationX = (int16_t) (accelerationBuffer[0] | (accelerationBuffer[1] << 8));
    int16_t accelerationY = (int16_t) (accelerationBuffer[2] | (accelerationBuffer[3] << 8));
    int16_t accelerationZ = (int16_t) (accelerationBuffer[4] | (accelerationBuffer[5] << 8));
    data.LinearAcceleration[X] = ((double) accelerationX) / 100.0;
    data.LinearAcceleration[Y] = ((double) accelerationY) / 100.0;
    data.LinearAcceleration[Z] = ((double) accelerationZ) / 100.0;

    __u8 eulerBuffer[6] = {};
    __u8 eulerRegister = EULER_H_LSB_REG;
    _i2c->Read(_sensorAddress, &eulerRegister, 1, eulerBuffer, 6);
    int16_t eulerX = (int16_t) (eulerBuffer[0] | (eulerBuffer[1] << 8));
    int16_t eulerY = (int16_t) (eulerBuffer[2] | (eulerBuffer[3] << 8));
    int16_t eulerZ = (int16_t) (eulerBuffer[4] | (eulerBuffer[5] << 8));
    data.EulerAngle[X] = ((double) eulerZ) / 16.0;
    data.EulerAngle[Y] = ((double) eulerY) / 16.0;
    data.EulerAngle[Z] = ((double) eulerX) / 16.0;

    data.Temperature = (int8_t) (_i2c->ReadByteFromRegister(_sensorAddress, TEMP_REG));

    SetData(data);

    return true;
}

Data BNO055_I2C::GetData() {
    this->_dataMutex.lock_shared();
    Data data = this->_data;
    this->_dataMutex.unlock_shared();
    return data;
}

bool BNO055_I2C::Reload() {
    return false;
}
