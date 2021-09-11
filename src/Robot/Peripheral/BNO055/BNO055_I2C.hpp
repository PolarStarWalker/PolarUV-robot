#ifndef ROBOT_BNO055_I2C_HPP
#define ROBOT_BNO055_I2C_HPP

#include <cstdint>
#include <array>
#include "./BNO055.hpp"
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../Interfaces/II2CPeriperal.hpp"

class BNO055_I2C : public II2CPerephiral {
public:

    explicit BNO055_I2C(uint16_t bnoAddress, OperationMode mode = OPERATION_MODE_NDOF);

    bool Init(const I2C *i2c) final;

    void SetOperationMode(OperationMode mode);

    void UseExternalCrystal(bool useExtCrl);

    inline std::array<double,3> GetEulerAngle(){
        this->_dataMutex.lock_shared();
        std::array<double,3> eulerAngle = this->_data.EulerAngle;
        this->_dataMutex.unlock_shared();
        return eulerAngle;
    }

private:
    I2C *_i2c;
    uint16_t _sensorAddress;
    OperationMode _operationMode;
    Data _data;
    std::shared_mutex _dataMutex;

    ///CalibrationData
    std::array<uint8_t, 4> _calibrationArray{};

    bool ReadData() const final;

    bool Restart() const final;
};

#endif
