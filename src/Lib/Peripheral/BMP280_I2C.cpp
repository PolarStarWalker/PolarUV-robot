#include "BMP280/BMP280_I2C.hpp"

SensorTask BMP280_I2C::ReadData() {
    for (;;){
        co_yield false;
        co_await TimerType::SleepFor_ms(100);
    }
}

SensorTask BMP280_I2C::Init() {
    for (;;){
        co_yield false;
        co_await TimerType::SleepFor_ms(100);
    }
}

BMP280_I2C::BMP280_I2C(I2C &i2c, uint16_t address) :
        ISensor(i2c),
        address_(address) {}
