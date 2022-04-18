#ifndef ROV_II2CPERIFERAL_HPP
#define ROV_II2CPERIFERAL_HPP

#include <shared_mutex>
#include "../../DataTransmissions/I2C/I2C.hpp"

class PeripheralHandler;
class I2CSensorsContext;

class II2CPeripheral {
    friend PeripheralHandler;
    friend I2CSensorsContext;
protected:

    explicit II2CPeripheral(ssize_t period_us) : period_us_(period_us){}

    virtual bool ReadData() = 0;

    virtual bool Init(const I2C* i2c) = 0;

    virtual bool Reload() = 0;
    virtual size_t DelayUs() const = 0;

    const ssize_t period_us_;
};

#endif