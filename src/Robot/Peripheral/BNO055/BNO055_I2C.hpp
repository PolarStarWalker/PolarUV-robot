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

    BNO055::Data GetData();

private:
    I2C *_i2c;
    uint16_t _sensorAddress;
    BNO055::OperationMode _operationMode{};
    BNO055::Data _data{};
    std::mutex _dataMutex;

    bool ReadData() final;

    bool WriteData() final;

    bool Reload() final;
};

#endif
