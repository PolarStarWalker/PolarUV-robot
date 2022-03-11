#ifndef ROBOT_MS5837_HPP
#define ROBOT_MS5837_HPP
namespace MS5837 {

    enum Commands : uint16_t {
        MS5837_RESET = 0x1E,
        MS5837_ADC_READ = 0x00,
        MS5837_PROM_READ = 0xA0,
        MS5837_CONVERT_D1_8192 = 0x4A,
        MS5837_CONVERT_D2_8192 = 0x5A
    };

    struct Data {
        double Pressure = 0.0f;
        double Depth = 0.0f;
        double Temperature = 0.0f;
    };

    enum FiltersAxis : size_t{
        Depth = 0,
        Pressure = 1,
        Temperature = 2
    };

}

#endif