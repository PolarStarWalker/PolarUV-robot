#ifndef ROBOT_II2CPERIPERAL_HPP
#define ROBOT_II2CPERIPERAL_HPP

#include "../../DataTransmissions/I2C/I2C.hpp"

class PeripheralHandler;

class II2CPeripheral {
    friend PeripheralHandler;

private:

    virtual bool Init(const I2C* i2c) = 0;

    virtual bool ReadData() const = 0;

    virtual bool Restart() const = 0;

    const I2C *_i2c;
};

#endif
