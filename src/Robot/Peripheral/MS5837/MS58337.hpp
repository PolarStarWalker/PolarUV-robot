#ifndef ROBOT_MS58337_HPP
#define ROBOT_MS58337_HPP
namespace MS5837{
    enum Commands : uint8_t {
        MS5837_RESET = 0x1E,
        MS5837_ADC_READ = 0x00,
        MS5837_PROM_READ = 0xA0,
        MS5837_CONVERT_D1_8192 = 0x4A,
        MS5837_CONVERT_D2_8192 = 0x5A
    };

    enum Data{

    };
}
#endif //ROBOT_MS58337_HPP
