#ifndef ROBOT_ADS1115_I2C_HPP
#define ROBOT_ADS1115_I2C_HPP

#include "Peripheral/PeripheralHandler/SensorHandler.hpp"
#include "./ADS1115.hpp"

class ADS1115_I2C final : public ISensor {
private:
    ADS1115::Gain gain_;
    uint16_t dataRate_;
    uint16_t address_;

public:
    explicit ADS1115_I2C(I2C &i2c, uint16_t address, ADS1115::Gain gain, ADS1115::DataRate dataRate);

    int16_t ReadADC_SingleEnded(ADS1115::Channel channel);

    void StartComparator_SingleEnded(ADS1115::Channel channel, int16_t threshold);

    int16_t GetLastConversionResults();

    double ComputeVolts(int16_t counts);

private:
    inline bool ConversionComplete(){
        return (i2c_->ReadByteFromRegister(address_, ADS1115::CONFIG).first & 0x8000) != 0;
    }

    inline void WriteRegister(uint8_t reg, uint16_t value) {
        i2c_->WriteByteToRegister(address_, reg, value);
    }

public:

    SensorTask Init();

    SensorTask ReadData();
};

#endif
