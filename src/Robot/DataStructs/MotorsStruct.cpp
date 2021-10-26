#include "./MotorsStruct/MotorsStruct.hpp"

std::ostream &operator<<(std::ostream &ostream, const MotorsStruct &motorsStruct) {
    ostream << "[MotorsCommand]"
            << std::endl

            << "High PWM: ["
            << motorsStruct.HiPWM[0] << ", "
            << motorsStruct.HiPWM[1] << ", "
            << motorsStruct.HiPWM[2] << ", "
            << motorsStruct.HiPWM[3] << ", "
            << motorsStruct.HiPWM[4] << ", "
            << motorsStruct.HiPWM[5] << ", "
            << motorsStruct.HiPWM[6] << ", "
            << motorsStruct.HiPWM[7] << ", "
            << motorsStruct.HiPWM[8] << ", "
            << motorsStruct.HiPWM[9] << ", "
            << motorsStruct.HiPWM[10] << ", "
            << motorsStruct.HiPWM[11] << "]"
            << std::endl

            << "Low PWM: ["
            << motorsStruct.LowPWM[0] << ", "
            << motorsStruct.LowPWM[1] << ", "
            << motorsStruct.LowPWM[2] << ", "
            << motorsStruct.LowPWM[3] << "]"
            << std::endl


            << "Time Prescaler: ["
            << motorsStruct.TimerPrescaler << "]"
            << std::endl;

    return ostream;
}