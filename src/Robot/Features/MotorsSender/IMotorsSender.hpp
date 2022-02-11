#ifndef ROBOT_IMOTORSSENDER_HPP
#define ROBOT_IMOTORSSENDER_HPP

namespace MotorsSender {

    struct MotorsStruct {
        enum DShotMode : int8_t {
            DShot150 = 1,
            DShot300 = 2,
            DShot600 = 4,
            DShot1200 = 8
        };

        uint16_t HiPWM[12]{};
        uint16_t LowPWM[4]{};

        ///DShotMode applies to the entire timer,
        ///it changes the operation of all channels of one timer
        ///Array prototype
        ///{T1, T2, T3, T4, T5}
        DShotMode TimerPrescaler = DShot300;
    };

    constexpr size_t MotorsStructArrayLength = sizeof(MotorsStruct::HiPWM);
    constexpr size_t MotorsStructLen = sizeof(MotorsStruct);
    constexpr size_t MotorsStructLenMessage = MotorsStructLen + 4;

    enum Id : size_t {
        SPI = 0
    };

    class IMotorsSender {
    public:

        virtual bool SendMotorsStruct(const MotorsStruct &motorsStruct) const = 0;

        virtual ~IMotorsSender() = default;
    };

};
#endif