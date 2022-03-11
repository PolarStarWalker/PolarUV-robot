#ifndef ROBOT_IMOTORSSENDER_HPP
#define ROBOT_IMOTORSSENDER_HPP

#include <cstdint>
#include <ostream>

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

        friend std::ostream &operator<<(std::ostream &out, const MotorsStruct &motors) {

            out << "[MOTORS STRUCT]\n"
                << "HiPWM: "
                << motors.HiPWM[0] << ", "
                << motors.HiPWM[1] << ", "
                << motors.HiPWM[2] << ", "
                << motors.HiPWM[3] << ", "
                << motors.HiPWM[4] << ", "
                << motors.HiPWM[5] << ", "
                << motors.HiPWM[6] << ", "
                << motors.HiPWM[7] << ", "
                << motors.HiPWM[8] << ", "
                << motors.HiPWM[9] << ", "
                << motors.HiPWM[10] << ", "
                << motors.HiPWM[11] << ", "
                << '\n'
                << "LoPWM: "
                << motors.LowPWM[0] << ", "
                << motors.LowPWM[1] << ", "
                << motors.LowPWM[2] << ", "
                << motors.LowPWM[3];

            return out;
        }
    };

    constexpr std::size_t MotorsStructArrayLength = sizeof(MotorsStruct::HiPWM);
    constexpr std::size_t MotorsStructLen = sizeof(MotorsStruct);
    constexpr std::size_t MotorsStructLenMessage = MotorsStructLen + 4;

    enum Id : std::size_t {
        SPI = 0
    };

    class IMotorsSender {
    public:

        virtual bool SendMotorsStruct(const MotorsStruct &motorsStruct) const = 0;

        virtual ~IMotorsSender() = default;
    };

};
#endif