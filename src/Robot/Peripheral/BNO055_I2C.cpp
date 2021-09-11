#include "BNO055/BNO055_I2C.hpp"

BNO055_I2C::BNO055_I2C(uint16_t bnoAddress, OperationMode mode = OPERATION_MODE_NDOF) {
    this->_bnoAddress = bnoAddress;
    this->_operationMode = mode;
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
    SetOperationMode(OPERATION_MODE_CONFIG);

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
    SetOperationMode(mode);
    usleep(20 * 1000);

    return true;
}

void BNO055_I2C::SetOperationMode(BNO055_I2C::OperationMode mode) {
    _operationMode = mode;
    _i2c->WriteByteToRegister(_sensorAddress, OPR_MODE_REG, _operationMode);
    usleep(30 * 1000);
}

void BNO055_I2C::UseExternalCrystal(bool useExtCrl) {
    OperationMode previousMode = _operationMode;

    SetOperationMode(OPERATION_MODE_CONFIG);
    usleep(25 * 1000);

    _i2c->WriteByteToRegister(_sensorAddress, PAGE_ID_REG, 0);

    useExtCrl ?
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x80) :
    _i2c->WriteByteToRegister(_sensorAddress, SYS_TRIGGER_REG, 0x00);

    usleep(10 * 1000);

    SetOperationMode(previousMode);
    usleep(20 * 1000);
}

bool BNO055_I2C::ReadData() {

    uint8_t calibrationData = _i2c->ReadByteFromRegister(_sensorAddress, CALIB_STAT_REG);
    this->CalibrationArray[0] = (uint8_t) ((calibrationData >> 6) & 0x03); // System
    this->_data.calibrationArray[1] = (uint8_t) ((calibrationData >> 4) & 0x03); // Gyroscope
    this->_data.calibrationArray[2] = (uint8_t) ((calibrationData >> 2) & 0x03); // Accelerometer
    this->_data.calibrationArray[3] = (uint8_t) (calibrationData & 0x03);        // Magnetometer

    __u8 accelerationBuffer[6] = {};
    __u8 accelerationRegister = LINEAR_ACCEL_DATA_X_LSB_REG;
    _i2c->Read(_sensorAddress, &accelerationRegister, 1, accelerationBuffer, 6);
    auto accelerationX = (int16_t) (accelerationBuffer[0] | (accelerationBuffer[1] << 8));
    auto accelerationY = (int16_t) (accelerationBuffer[2] | (accelerationBuffer[3] << 8));
    auto accelerationZ = (int16_t) (accelerationBuffer[4] | (accelerationBuffer[5] << 8));
    this->_data.accelerationArray[0] = ((double) accelerationX) / 100.0;
    this->_data.accelerationArray[1] = ((double) accelerationY) / 100.0;
    this->_data.accelerationArray[2] = ((double) accelerationZ) / 100.0;

    __u8 eulerBuffer[6] = {};
    __u8 eulerRegister = EULER_H_LSB_REG;
    _i2c->Read(_sensorAddress, &eulerRegister, 1, eulerBuffer, 6);
    auto eulerX = (int16_t) (eulerBuffer[0] | (eulerBuffer[1] << 8));
    auto eulerY = (int16_t) (eulerBuffer[2] | (eulerBuffer[3] << 8));
    auto eulerZ = (int16_t) (eulerBuffer[4] | (eulerBuffer[5] << 8));
    this->_data.eulerArray[0] = ((double) eulerX) / 16.0;
    this->_data.eulerArray[1] = ((double) eulerY) / 16.0;
    this->_data.eulerArray[2] = ((double) eulerZ) / 16.0;

    this->_data.temperature = (int8_t) (_i2c->ReadByteFromRegister(_sensorAddress, TEMP_REG));

    return true;
}
