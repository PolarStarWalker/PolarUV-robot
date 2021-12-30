#ifndef ROBOT_ADS1115_I2C_HPP
#define ROBOT_ADS1115_I2C_HPP

#include "./ADS1115.hpp"
#include "../Interfaces/II2CPeripheral.hpp"

class ADS1115_I2C final : public II2CPeripheral {
private:
    const I2C *_i2c;
    ADS1115::Gain _gain;
    uint16_t _dataRate;
    uint16_t _address;

public:
    explicit ADS1115_I2C(uint16_t address = ADS1X15_ADDRESS,
                         ADS1115::Gain gain = ADS1115::GAIN_TWOTHIRDS,
                         ADS1115::DataRate dataRate = ADS1115::SPS128);

    bool Init(const I2C *i2c) final;

    bool ReadData() final;

    bool Reload() final;

    int16_t ReadADC_SingleEnded(ADS1115::Channel channel);

    void StartComparator_SingleEnded(ADS1115::Channel channel, int16_t threshold);

    int16_t GetLastConversionResults();

    double ComputeVolts(int16_t counts);

private:
    inline bool ConversionComplete(){
        return (_i2c->ReadByteFromRegister(_address, ADS1115::CONFIG) & 0x8000) != 0;
    }

    inline void WriteRegister(uint8_t reg, uint16_t value) {
        _i2c->Write(_address, &reg, 1, (uint8_t *) &value, 2);
    }
};

#endif
