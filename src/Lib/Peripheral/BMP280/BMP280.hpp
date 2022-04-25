#ifndef ROBOT_BMP280_HPP
#define ROBOT_BMP280_HPP

namespace BMP280{
    enum Register : uint8_t{
        DIG_T1 = 0x88,
        DIG_T2 = 0x8A,
        DIG_T3 = 0x8C,
        DIG_P1 = 0x8E,
        DIG_P2 = 0x90,
        DIG_P3 = 0x92,
        DIG_P4 = 0x94,
        DIG_P5 = 0x96,
        DIG_P6 = 0x98,
        DIG_P7 = 0x9A,
        DIG_P8 = 0x9C,
        DIG_P9 = 0x9E,
        CHIPID = 0xD0,
        VERSION = 0xD1,
        SOFTRESET = 0xE0,
        CAL26 = 0xE1, /**< R calibration = 0xE1-0xF0 */
        STATUS = 0xF3,
        CONTROL = 0xF4,
        CONFIG = 0xF5,
        PRESSUREDATA = 0xF7,
        TEMPDATA = 0xFA,
    };
}

#endif