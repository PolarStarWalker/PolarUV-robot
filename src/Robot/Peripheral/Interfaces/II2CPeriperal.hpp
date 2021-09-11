#ifndef ROBOT_II2CPERIPERAL_HPP
#define ROBOT_II2CPERIPERAL_HPP

#include "../../DataTransmissions/I2C/I2C.hpp"

class PeripheralHandler;

class II2CPeripheral {
    friend PeripheralHandler;

    //ToDo: делать private
public:

    virtual bool Init(I2C* i2c) = 0;

    virtual bool ReadData() = 0;

    virtual bool Restart() = 0;
};

#endif
