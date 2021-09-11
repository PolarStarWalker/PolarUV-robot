#pragma once

#include "../Interfaces/II2CPeriperal.hpp"

#define MS5837_ADDRESS 0x76

class MS5837_I2C : II2CPeripheral {
public:

    explicit MS5837_I2C(const char *i2cDevice, uint16_t MS5837Address);

    ~MS5837_I2C();

    void SetFluidDensity(double density);

    //from sensor
    bool Initialize();

private:

    I2C *_i2c;

    uint16_t C[8]{};
    uint32_t _d1Pressure{};
    uint32_t _d2Temperature{};
    int32_t _temperature{};
    int32_t _p{};

    double _fluidDensity = 997.0; // Freshwater

    static uint8_t CRC4(uint16_t *n_prom);

    void Calculate();

};
