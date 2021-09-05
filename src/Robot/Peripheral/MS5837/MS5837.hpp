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

    enum MS5837Models : uint8_t {
        MS5837_30BA = 0,
        MS5837_02BA = 1,
        MS5837_UNRECOGNISED = 255
    };

    enum MS5837Versions : uint8_t {
        MS5837_02BA01 = 0x00,
        MS5837_02BA21 = 0x15,
        MS5837_30BA26 = 0x1A
    };

    explicit MS5837(const char *i2cDevice, uint16_t MS5837Address);

    MS5837Data GetData();

    //from sensor
    bool Initialize();

    uint8_t GetId() { return this->_address; }

private:

    MS5837Data _data;

    I2C *_i2c;

    uint16_t C[8]{};
    uint32_t _d1Pressure{};
    uint32_t _d2Temperature{};
    int32_t _temperature{};
    int32_t _p{};
    uint8_t _model{};

    double _fluidDensity = 997.0; // Freshwater

    static uint8_t CRC4(uint16_t *n_prom);

    void SetFluidDensity(double density);

    void Calculate();

    //from interface
    bool ReadData() final;

    bool WriteData() final;

    bool Reload() final;
};
