#ifndef ROBOT_BNO055_I2C_HPP
#define ROBOT_BNO055_I2C_HPP

#include <cstdint>
#include <array>
#include <shared_mutex>
#include "./BNO055.hpp"
#include "../Interfaces/II2CPeriperal.hpp"
#include "../../DataTransmissions/I2C/I2C.hpp"

class BNO055_I2C : public II2CPeripheral {
public:

    explicit BNO055_I2C(uint16_t bnoAddress, BNO055::OperationMode mode = BNO055::OPERATION_MODE_NDOF);

    bool Init(I2C *i2c) final;

    void SetOperationMode(BNO055::OperationMode mode);

    void UseExternalCrystal(bool useExtCrl);

    inline std::array<double, 3> GetEulerAngle() {
        this->_dataMutex.lock_shared();
        std::array<double, 3> eulerAngle = this->_data.EulerAngle;
        this->_dataMutex.unlock_shared();
        return eulerAngle;
    }

    inline std::array<double, 3> GetLinearAcceleration() {
        this->_dataMutex.lock_shared();
        std::array<double, 3> linearAcceleration = this->_data.LinearAcceleration;
        this->_dataMutex.unlock_shared();
        return linearAcceleration;
    }

    inline std::array<uint8_t, 4> GetCalibration() {
        this->_dataMutex.lock_shared();
        std::array<uint8_t, 4> calibration = this->_data.CalibrationArray;
        this->_dataMutex.unlock_shared();
        return calibration;
    }

    bool ReadData() final;

    bool Restart() final;

private:
    BNO055::Data _data;
    std::shared_mutex _dataMutex;
    I2C *_i2c;
    uint16_t _sensorAddress;
    BNO055::OperationMode _operationMode;

};

#endif
