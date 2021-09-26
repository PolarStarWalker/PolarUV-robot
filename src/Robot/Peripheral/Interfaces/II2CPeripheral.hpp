#ifndef ROV_II2CPERIFERAL_HPP
#define ROV_II2CPERIFERAL_HPP

#include <shared_mutex>
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../../DataTransmissions/SPI/SPI.hpp"
#include "../../DataTransmissions/UART/UART.hpp"

class PeripheralHandler;

class II2CPeripheral {
    friend PeripheralHandler;
protected:

    virtual bool ReadData() = 0;

    virtual bool Init(const I2C* i2c) = 0;

    virtual bool Reload() = 0;
};

#endif