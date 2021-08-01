#include "./MotorsStruct/MotorsStruct.hpp"

std::ostream &operator<<(std::ostream &ostream, const MotorsStruct &motorsStruct) {
    ostream << "MotorsCommand: ["
            << motorsStruct.PacketArray[0] << ", "
            << motorsStruct.PacketArray[1] << ", "
            << motorsStruct.PacketArray[2] << ", "
            << motorsStruct.PacketArray[3] << ", "
            << motorsStruct.PacketArray[4] << ", "
            << motorsStruct.PacketArray[5] << ", "
            << motorsStruct.PacketArray[6] << ", "
            << motorsStruct.PacketArray[7] << ", "
            << motorsStruct.PacketArray[8] << ", "
            << motorsStruct.PacketArray[9] << ", "
            << motorsStruct.PacketArray[10] << ", "
            << motorsStruct.PacketArray[11] << "]"
            << std::endl

            << "Time Prescaler: ["
            << motorsStruct.TimerPrescaler << "]"
      << std::endl;

    return ostream;
}