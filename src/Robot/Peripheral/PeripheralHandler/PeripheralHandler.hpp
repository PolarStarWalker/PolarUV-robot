#ifndef ROBOT_PERIPHERAL_HANDLER_HPP
#define ROBOT_PERIPHERAL_HANDLER_HPP

#include <thread>
#include <cstring>
#include <mutex>
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

    std::mutex _i2cMutex;
    std::mutex _i2cSensorsMutex;

    std::list<I2CSensorsContext> _i2cPeripherals;

    I2C _i2c;

public:

    PeripheralHandler(const char *i2c);

    bool AddI2CSensor(II2CPeripheral *newSensor);

    void Start();

    void StartAsync();

};

#endif