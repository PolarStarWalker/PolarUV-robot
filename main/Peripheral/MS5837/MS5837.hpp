#pragma once

#include "../interfaces/IPeripheral.hpp"
#include "MS5837Data.hpp"

class MS5837 : IPeripheral, MS5837Registers {
private:

    MS5837Data _data;

    //from interface
    bool ReadData() final;

    bool WriteData() final;

    bool Reload() final;

public:

    explicit MS5837(uint8_t address);

    MS5837Data GetDepth();

    //from sensor
    bool Initialize();

    uint8_t GetId() { return this->_address; }
};
