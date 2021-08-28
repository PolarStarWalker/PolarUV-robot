#include "./SensorsHandler/SensorsHandler.hpp"

void SensorsHandler::AddSensor(IPeripheral *newSensor) {
    this->_peripherals.push_back(newSensor);
}

SensorsHandler::SensorsHandler(char *i2c, char *uart, UART::SpeedEnum uartSpeed, char *spi, u_int32_t spiSpeedHz) :
        _i2c(i2c),
        _uart(uart, uartSpeed),
        _spi(spi, spiSpeedHz) {}


void Start(){

}