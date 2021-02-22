#include "./ADS1115/ADS1115_I2C.hpp"
#include "./Math/SIPrefix.hpp"

using namespace ADS1115;

ADS1115_I2C::ADS1115_I2C(I2C &i2c, uint16_t address, ADS1115::Gain gain, ADS1115::DataRate dataRate)
        : ISensor(i2c),
          gain_(gain),
          dataRate_(dataRate),
          address_(address) {}

SensorTask ADS1115_I2C::Init() {
    for (;;) {
        co_yield false;
        co_await TimerType::SleepFor_ms(100);
    }
}

SensorTask ADS1115_I2C::ReadData() {
    for (;;) {
        co_yield false;
        co_await TimerType::SleepFor_ms(100);
    }
}

int16_t ADS1115_I2C::ReadADC_SingleEnded(Channel channel) {
    if (channel > 3)
        return 0;

    uint16_t config = CQUE_NONE |
                      CLAT_NONLAT |
                      CPOL_ACTVLOW |
                      CMODE_TRAD |
                      MODE_SINGLE;

    config |= gain_ |
              dataRate_; ///ToDo:??????

    switch (channel) {
        case (A0):
            config |= MUX_SINGLE_0;
            break;
        case (A1):
            config |= MUX_SINGLE_1;
            break;
        case (A2):
            config |= MUX_SINGLE_2;
            break;
        case (A3):
            config |= MUX_SINGLE_3;
            break;
    }

    config |= OS_SINGLE;

    WriteRegister(CONFIG, config);

    while (!ConversionComplete());

    return GetLastConversionResults();
}

void ADS1115_I2C::StartComparator_SingleEnded(Channel channel,
                                              int16_t threshold) {
    uint16_t config = CQUE_1CONV |
                      CLAT_LATCH |
                      CPOL_ACTVLOW |
                      CMODE_TRAD |
                      MODE_CONTIN;

    config |= gain_ |
              dataRate_;

    switch (channel) {
        case (A0):
            config |= MUX_SINGLE_0;
            break;
        case (A1):
            config |= MUX_SINGLE_1;
            break;
        case (A2):
            config |= MUX_SINGLE_2;
            break;
        case (A3):
            config |= MUX_SINGLE_3;
            break;
    }

    WriteRegister(HITHRESH, threshold);

    WriteRegister(CONFIG, config);
}

int16_t ADS1115_I2C::GetLastConversionResults() {
    uint16_t result = i2c_->ReadByteFromRegister(address_, CONVERT).first;
    return (int16_t) result;
}

double ADS1115_I2C::ComputeVolts(int16_t counts) {
    double fsRange;
    switch (gain_) {
        case GAIN_TWOTHIRDS:
            fsRange = 6.144f;
            break;
        case GAIN_ONE:
            fsRange = 4.096f;
            break;
        case GAIN_TWO:
            fsRange = 2.048f;
            break;
        case GAIN_FOUR:
            fsRange = 1.024f;
            break;
        case GAIN_EIGHT:
            fsRange = 0.512f;
            break;
        case GAIN_SIXTEEN:
            fsRange = 0.256f;
            break;
        default:
            fsRange = 0.0f;
    }
    return counts * (fsRange / (0x8000));
}




