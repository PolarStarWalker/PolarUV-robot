#pragma once

#include <thread>
#include <cstring>
#include <mutex>
#include <list>
#include "../Interfaces/II2CPeriperal.hpp"

#include "../../DataTransmissions/UART/UART.hpp"
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../../DataTransmissions/SPI/SPI.hpp"

struct I2CSensorsContext {

    explicit I2CSensorsContext(II2CPeripheral *i2cPerephiral) {
        I2CPerephiral = i2cPerephiral;
        IsOnline = false;
        WaitTransmissions = false;
    }

    const II2CPeripheral *I2CPerephiral;
    bool IsOnline;
    bool WaitTransmissions;
};

class PeripheralHandler {
private:
    std::mutex _arrayMutex;
    std::mutex _i2cMutex;
    std::mutex _i2cSensorsMutex;

    std::list<I2CSensorsContext> _i2cPeripherals;
    size_t _sensorsNumber = 0;

    I2C _i2c;
    SPI _spi;
    UART _uart;

public:

    PeripheralHandler(const char *i2c, const char *uart, UART::SpeedEnum uartSpeed, const char *spi, u_int32_t spiSpeedHz = 5000000);

    bool AddI2CSensor(II2CPeripheral *newSensor);

    void Start();

    void StartAsync();

};