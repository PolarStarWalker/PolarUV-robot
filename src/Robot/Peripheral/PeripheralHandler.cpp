#include "./SensorsHandler/PeripheralHandler.hpp"


bool PeripheralHandler::AddI2CSensor(II2CPerephiral *newSensor) {
    this->_i2cMutex.lock();
    bool isInit = newSensor->Init(&this->_i2c);
    this->_i2cMutex.unlock();

    if (isInit) {
        this->_i2cSensorsMutex.lock();
        this->_i2cPeripherals.emplace_back(newSensor);
        this->_i2cSensorsMutex.unlock();
    }

    return isInit;
}

PeripheralHandler::PeripheralHandler(const char *i2c,
                               const char *uart,
                               UART::SpeedEnum uartSpeed,
                               const char *spi,
                               u_int32_t spiSpeedHz) :
        _i2c(i2c),
        _uart(uart, uartSpeed),
        _spi(spi, spiSpeedHz) {}

void PeripheralHandler::Start() {
    for (;;) {
        for (I2CSensorsContext peripheralContext : this->_i2cPeripherals) {
            peripheralContext.IsOnline = peripheralContext.I2CPerephiral->ReadData();
        }
        usleep(20 * 1000);
    }
}
