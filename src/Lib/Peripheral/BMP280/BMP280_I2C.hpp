#ifndef ROBOT_BMP280_I2C_HPP
#define ROBOT_BMP280_I2C_HPP

#include "BMP280.hpp"

#include "../PeripheralHandler/ISensor.hpp"

constexpr uint16_t BMP280_ADDRESS = 0x77;
constexpr uint16_t BMP280_CHIPID = 0x58;

class BMP280_I2C final : public ISensor {
public:
    explicit BMP280_I2C(I2C &i2c, uint16_t address);

private:
    const uint16_t address_;

public:
    SensorTask ReadData();

    SensorTask Init();
};

#endif