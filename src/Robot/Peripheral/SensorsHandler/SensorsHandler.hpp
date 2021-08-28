#pragma once

#include <thread>
#include <cstring>
#include <mutex>
#include <list>
#include "../Interfaces/IPeripheral.hpp"


class SensorsHandler {
private:
	std::mutex _arrayMutex;
	
	std::list<IPeripheral*> _peripherals;
	size_t _sensorsNumber = 0;

	I2C _i2c;
	SPI _spi;
	UART _uart;

public:

    SensorsHandler(char *i2c, char *uart, UART::SpeedEnum uartSpeed, char *spi, u_int32_t spiSpeedHz = 5000000);

	void AddSensor(IPeripheral*);

	void Start();

};