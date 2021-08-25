#include "BNO055/BNO055.hpp"

BNO055::BNO055(const char *i2cAddress, uint16_t bnoAddress) {
    _i2c = new I2C(i2cAddress);
    _bnoAddress = bnoAddress;
}

BNO055::~BNO055() {

}

bool BNO055::begin(operationMode_t mode) {

    /// Connection test
    uint8_t id = _i2c->ReadByte(_bnoAddress, CHIP_ID_REG);
    if (id != BNO055_ID) {
        usleep(1000 * 1000);
        id = _i2c->ReadByte(_bnoAddress, CHIP_ID_REG);
        if (id != BNO055_ID) {
            return false;
        }
    }

    /// Switch to config mode
    setOperationMode(OPERATION_MODE_CONFIG);

    /// Reset
    _i2c->WriteByte(_bnoAddress, SYS_TRIGGER_REG, 0x20);
    usleep(30 * 1000);
    while (_i2c->ReadByte(_bnoAddress, CHIP_ID_REG) != BNO055_ID) {
        usleep(10 * 1000);
    }
    usleep(50 * 1000);

    /// Set to normal power mode
    _i2c->WriteByte(_bnoAddress, PWR_MODE_REG, POWER_MODE_NORMAL);
    usleep(10 * 1000);
    _i2c->WriteByte(_bnoAddress, PAGE_ID_REG, 0);

    /// Set the output units
//    uint8_t unitsel = (0 << 7) | // Orientation = Android
//                      (0 << 4) | // Temperature = Celsius
//                      (0 << 2) | // Euler = Degrees
//                      (1 << 1) | // Gyro = Rads
//                      (0 << 0);  // Accelerometer = m/s^2
//    _i2c->WriteByte(_bnoAddress,UNIT_SEL_REG, unitsel);

    /// Set the requested operation mode
    _i2c->WriteByte(_bnoAddress, SYS_TRIGGER_REG, 0x0);
    usleep(10 * 1000);
    setOperationMode(mode);
    usleep(20 * 1000);

    return true;
}

void BNO055::setOperationMode(BNO055::operationMode_t mode) {
    _operationMode = mode;
    _i2c->WriteByte(_bnoAddress, OPR_MODE_REG, _operationMode);
    usleep(30 * 1000);
}

void BNO055::useExternalCrystal(bool useExtCrl) {
    operationMode_t previousMode = _operationMode;

    setOperationMode(OPERATION_MODE_CONFIG);
    usleep(25 * 1000);

    _i2c->WriteByte(_bnoAddress, PAGE_ID_REG, 0);
    if (useExtCrl) {
        _i2c->WriteByte(_bnoAddress, SYS_TRIGGER_REG, 0x80);
    } else {
        _i2c->WriteByte(_bnoAddress, SYS_TRIGGER_REG, 0x00);
    }
    usleep(10 * 1000);

    setOperationMode(previousMode);
    usleep(20 * 1000);
}

std::array<uint8_t, 4> BNO055::getCalibration() {
    uint8_t calibrationData = _i2c->ReadByte(_bnoAddress, CALIB_STAT_REG);
    std::array<uint8_t, 4> calibrationArray{
            (uint8_t) ((calibrationData >> 6) & 0x03), // System
            (uint8_t) ((calibrationData >> 4) & 0x03), // Gyroscope
            (uint8_t) ((calibrationData >> 2) & 0x03), // Accelerometer
            (uint8_t) (calibrationData & 0x03)         // Magnetometer
    };
    return calibrationArray;
}

std::array<double, 3> BNO055::getLinearAcceleration() {

    __u8 buffer[6] = {};
    __u8 slaveRegister = LINEAR_ACCEL_DATA_X_LSB_REG;

    _i2c->Read(_bnoAddress, &slaveRegister, 1, buffer, 6);

    int16_t x = ((int16_t) buffer[0]) | (((int16_t) buffer[1]) << 8);
    int16_t y = ((int16_t) buffer[2]) | (((int16_t) buffer[3]) << 8);
    int16_t z = ((int16_t) buffer[4]) | (((int16_t) buffer[5]) << 8);

    std::array<double, 3> dataArray = {};

    dataArray[0] = ((double) x) / 100.0;
    dataArray[1] = ((double) y) / 100.0;
    dataArray[2] = ((double) z) / 100.0;

    return dataArray;
}

std::array<double, 3> BNO055::getEulerAngles() {

    __u8 buffer[6] = {};
    __u8 slaveRegister = EULER_H_LSB_REG;

    _i2c->Read(_bnoAddress, &slaveRegister, 1, buffer, 6);

    int16_t x = ((int16_t) buffer[0]) | (((int16_t) buffer[1]) << 8);
    int16_t y = ((int16_t) buffer[2]) | (((int16_t) buffer[3]) << 8);
    int16_t z = ((int16_t) buffer[4]) | (((int16_t) buffer[5]) << 8);

    std::array<double, 3> dataArray = {};

    dataArray[0] = ((double) x) / 16.0;
    dataArray[1] = ((double) y) / 16.0;
    dataArray[2] = ((double) z) / 16.0;

    return dataArray;
}

int8_t BNO055::getTemperature() {
    auto temperature = (int8_t) (_i2c->ReadByte(_bnoAddress, TEMP_REG));
    return temperature;
}

