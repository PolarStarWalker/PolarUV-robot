#include "./MotorsStruct/MotorsStruct.hpp"

std::ostream &operator<<(std::ostream &ostream, const MotorsStruct &motorsStruct) {
    ostream << "MotorsCommand: ["
            << motorsStruct.HiSpeedPWM[0] << ", "
            << motorsStruct.HiSpeedPWM[1] << ", "
            << motorsStruct.HiSpeedPWM[2] << ", "
            << motorsStruct.HiSpeedPWM[3] << ", "
            << motorsStruct.HiSpeedPWM[4] << ", "
            << motorsStruct.HiSpeedPWM[5] << ", "
            << motorsStruct.HiSpeedPWM[6] << ", "
            << motorsStruct.HiSpeedPWM[7] << ", "
            << motorsStruct.HiSpeedPWM[8] << ", "
            << motorsStruct.HiSpeedPWM[9] << ", "
            << motorsStruct.HiSpeedPWM[10] << ", "
            << motorsStruct.HiSpeedPWM[11] << "]"
            << std::endl

            << "Time Prescaler: ["
            << motorsStruct.TimerPrescaler << "]"
      << std::endl;

    return ostream;
}