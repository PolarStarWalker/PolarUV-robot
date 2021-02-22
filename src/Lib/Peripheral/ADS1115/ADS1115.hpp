#ifndef ROBOT_ADS1115_HPP
#define ROBOT_ADS1115_HPP

#define ADS1X15_ADDRESS (0x48) ///< 1001 000 (ADDR = GND)

#include <cstdint>

namespace ADS1115 {
    enum PointerRegisters : uint8_t {
        MASK = 0x03,
        CONVERT = 0x00,
        CONFIG = 0x01,
        LOWTHRESH = 0x02,
        HITHRESH = 0x03
    };

    enum ConfigRegisters : uint16_t {
        OS_MASK = 0x8000,
        OS_SINGLE = 0x8000,
        OS_BUSY = 0x0000,
        OS_NOTBUSY = 0x8000,
        MUX_MASK = 0x7000,
        MUX_DIFF_0_1 = 0x0000,
        MUX_DIFF_0_3 = 0x1000,
        MUX_DIFF_1_3 = 0x2000,
        MUX_DIFF_2_3 = 0x3000,
        MUX_SINGLE_0 = 0x4000,
        MUX_SINGLE_1 = 0x5000,
        MUX_SINGLE_2 = 0x6000,
        MUX_SINGLE_3 = 0x7000,
        PGA_MASK = 0x0E00,
        PGA_6_144V = 0x0000,
        PGA_4_096V = 0x0200,
        PGA_2_048V = 0x0400,
        PGA_1_024V = 0x0600,
        PGA_0_512V = 0x0800,
        PGA_0_256V = 0x0A00,
        MODE_MASK = 0x0100,
        MODE_CONTIN = 0x0000,
        MODE_SINGLE = 0x0100,
        RATE_MASK = 0x00E0,
        CMODE_MASK = 0x0010,
        CMODE_TRAD = 0x0000,
        CMODE_WINDOW = 0x0010,
        CPOL_MASK = 0x0008,
        CPOL_ACTVLOW = 0x0000,
        CPOL_ACTVHI = 0x0008,
        CLAT_MASK = 0x0004,
        CLAT_NONLAT = 0x0000,
        CLAT_LATCH = 0x0004,
        CQUE_MASK = 0x0003,
        CQUE_1CONV = 0x0000,
        CQUE_2CONV = 0x0001,
        CQUE_4CONV = 0x0002,
        CQUE_NONE = (0x0003)
    };

    enum Gain : uint16_t {
        GAIN_TWOTHIRDS = PGA_6_144V,
        GAIN_ONE = PGA_4_096V,
        GAIN_TWO = PGA_2_048V,
        GAIN_FOUR = PGA_1_024V,
        GAIN_EIGHT = PGA_0_512V,
        GAIN_SIXTEEN = PGA_0_256V
    };

    enum DataRate : uint16_t {
        SPS8 = 0x0000,
        SPS16 = 0x0020,
        SPS32 = 0x0040,
        SPS64 = 0x0060,
        SPS128 = 0x0080, //(default)
        SPS250 = 0x00A0,
        SPS475 = 0x00C0,
        SPS860 = 0x00E0
    };

    struct Data {
        double Voltage[4]{};
    };

    enum Channel{
        A0 = 0,
        A1 = 1,
        A2 = 2,
        A3 = 3,
    };

}

#endif
