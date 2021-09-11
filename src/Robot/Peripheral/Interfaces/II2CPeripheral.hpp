#ifndef ROV_IPERIFERAL_HPP
#define ROV_IPERIFERAL_HPP
#include <mutex>
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../../DataTransmissions/SPI/SPI.hpp"
#include "../../DataTransmissions/UART/UART.hpp"

class II2CPeripheral {
protected:

    virtual bool ReadData() = 0;

    virtual bool Init(const I2C* i2c) = 0;

    virtual bool Reload() = 0;
};

#endif