#pragma once

#include "../Interfaces/II2CPeripheral.hpp"
#include "./MS5837.hpp"

#define MS5837_ADDRESS 0x76

class MS5837_I2C : public II2CPeripheral {
public:

    explicit MS5837_I2C(uint16_t sensorAddress);

    ~MS5837_I2C();

    void SetFluidDensity(double density);

    MS5837::Data GetData();

    //from sensor
    bool Init(const I2C *i2c) final;

private:

    MS5837::Data _data;

    const I2C *_i2c;

    uint16_t C[8]{};
    uint32_t _d1Pressure{};
    uint32_t _d2Temperature{};
    int32_t _temperature{};
    int32_t _p{};
    uint16_t _sensorAddress;

    double _fluidDensity = 997.0; // Freshwater

    static uint8_t CRC4(uint16_t *n_prom);

    void Calculate();

    bool ReadData() final;

    bool Reload() final;
};
