#pragma once

#include "../Interfaces/IPeripheral.hpp"
#include "MS5837Data.hpp"

#define MS5837_ADDRESS 0x76

class MS5837 : IPeripheral, MS5837Registers {
public:

    enum MS5837Commands : uint8_t {
        MS5837_RESET = 0x1E,
        MS5837_ADC_READ = 0x00,
        MS5837_PROM_READ = 0xA0,
        MS5837_CONVERT_D1_8192 = 0x4A,
        MS5837_CONVERT_D2_8192 = 0x5A
    };

    explicit MS5837(const char *i2cDevice, uint16_t MS5837Address);

    ~MS5837();

    void SetFluidDensity(double density);

    MS5837Data GetData();

    //from sensor
    bool Initialize();

private:

    MS5837Data _data;

    I2C *_i2c;

    uint16_t C[8]{};
    uint32_t _d1Pressure{};
    uint32_t _d2Temperature{};
    int32_t _temperature{};
    int32_t _p{};

    double _fluidDensity = 997.0; // Freshwater

    static uint8_t CRC4(uint16_t *n_prom);

    void Calculate();

    //from interface
    bool ReadData() final;

    bool WriteData() final;

    bool Reload() final;
};
