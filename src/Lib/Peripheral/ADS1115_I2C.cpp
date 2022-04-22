#include "./ADS1115/ADS1115_I2C.hpp"
#include "./Math/SIPrefix.hpp"
using namespace ADS1115;

ADS1115_I2C::ADS1115_I2C(uint16_t address, ADS1115::Gain gain, ADS1115::DataRate dataRate) : ISensor(Kilo(50)){
    _i2c = nullptr;
    _gain = gain;
    _dataRate = dataRate;
    _address = address;
}

bool ADS1115_I2C::Init(const I2C *i2c, TimerType &timer) {
    _i2c = i2c;
    return 0;
}

bool ADS1115_I2C::ReadData(TimerType &timer) {
    return 0;
}

int16_t ADS1115_I2C::ReadADC_SingleEnded(Channel channel) {
    if (channel > 3)
        return 0;

    uint16_t config = CQUE_NONE |
                      CLAT_NONLAT |
                      CPOL_ACTVLOW |
                      CMODE_TRAD |
                      MODE_SINGLE;

    config |= _gain |
              _dataRate; ///ToDo:??????

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

    config |= _gain |
              _dataRate;

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
    uint16_t result = _i2c->ReadByteFromRegister(_address, CONVERT).first;
    return (int16_t) result;
}

double ADS1115_I2C::ComputeVolts(int16_t counts) {
    double fsRange;
    switch (_gain) {
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




