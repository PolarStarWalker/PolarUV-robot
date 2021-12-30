#ifndef ROBOT_PERIPHERAL_HANDLER_HPP
#define ROBOT_PERIPHERAL_HANDLER_HPP

#include <thread>
#include <cstring>
#include <shared_mutex>
#include <list>
#include "../Interfaces/II2CPeripheral.hpp"

#include "../../DataTransmissions/UART/UART.hpp"
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../../DataTransmissions/SPI/SPI.hpp"

struct I2CSensorsContext {

    II2CPeripheral *I2CPeripheral;
    bool IsOnline;
    bool WaitTransmissions;

    explicit I2CSensorsContext(II2CPeripheral *i2cPeripheral) {
        I2CPeripheral = i2cPeripheral;
        IsOnline = false;
        WaitTransmissions = false;
    }
};

class PeripheralHandler {
private:

    mutable std::shared_mutex _i2cMutex;
    mutable std::shared_mutex _i2cSensorsMutex;

    mutable std::list<I2CSensorsContext> _i2cPeripherals;

    const I2C _i2c;

    const size_t _delay_us;
public:

    explicit PeripheralHandler(const char *i2c, size_t delay_us);

    bool AddI2CSensor(II2CPeripheral *newSensor) const;

    [[noreturn]] void Start() const;

    void StartAsync() const;

};

#endif