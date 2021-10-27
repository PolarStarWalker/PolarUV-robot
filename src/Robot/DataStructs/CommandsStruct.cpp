#include "./CommandsStruct/CommandsStruct.hpp"

std::ostream& operator<<(std::ostream &ostream, const CommandsStruct &commandsStruct) {
    ostream << "VectorArray: ["
            << commandsStruct.MoveVector[Fx] << ", "
            << commandsStruct.MoveVector[Fy] << ", "
            << commandsStruct.MoveVector[Fz] << ", "
            << commandsStruct.MoveVector[Mx] << ", "
            << commandsStruct.MoveVector[My] << ", "
            << commandsStruct.MoveVector[Mz] << "]"
            << std::endl

            << "The Hand: ["
            << commandsStruct.TheHand[0] << ", "
            << commandsStruct.TheHand[1] << ", "
            << commandsStruct.TheHand[2] << ", "
            << commandsStruct.TheHand[3] << ", "
            << commandsStruct.TheHand[4] << ", "
            << commandsStruct.TheHand[5] << "]"
            << std::endl

            << "LowPWM: ["
            << commandsStruct.LowPWM[0] << ", "
            << commandsStruct.LowPWM[1] << ", "
            << commandsStruct.LowPWM[2] << ", "
            << commandsStruct.LowPWM[3] << "]"
            << std::endl

            << "MotorsLock: "
            << commandsStruct.MotorsLock << std::endl

            << "Stabilization: "
            << commandsStruct.Stabilization << std::endl;

    return ostream;
}