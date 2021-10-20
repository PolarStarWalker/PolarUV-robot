#ifndef ROBOT_MOTORSSTRUCT_HPP
#define ROBOT_MOTORSSTRUCT_HPP
#include <iostream>

enum DShotMode : int8_t {
    DShot150 = 1,
    DShot300 = 2,
    DShot600 = 4,
    DShot1200 = 8
};

struct MotorsStruct {
    /// Array prototype
    ///{T1CH1, T1CH2, T1CH3, T1CH4, T2CH1, T2CH2, T3CH3, T4CH1, T4CH2, T5CH1, T5CH2, T5CH4}
    ///or
    ///{PA8,   PA9,   PA10,  PA11,  PA15,  PB3,   PB0,   PB6,   PB7,   PA0,   PA1,   PA3}

    uint16_t PacketArray[12] = {};

    ///DShotMode applies to the entire timer,
    ///it changes the operation of all channels of one timer
    ///Array prototype
    ///{T1, T2, T3, T4, T5}

    DShotMode TimerPrescaler = DShot300;
};

extern MotorsStruct MotorsStructData;
constexpr size_t MotorsStructArrayLength = sizeof(MotorsStructData.PacketArray);
constexpr size_t MotorsStructLen = sizeof(MotorsStructData);
constexpr size_t MotorsStructLenMessage = MotorsStructLen + 4;

std::ostream &operator<<(std::ostream &s, const MotorsStruct &c);
#endif
