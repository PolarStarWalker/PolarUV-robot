#include "./CommandsStruct/CommandsStruct.hpp"

std::ostream& operator<<(std::ostream &ostream, const CommandsStruct &commandsStruct) {
    ostream << "MoveVector: ["
            << commandsStruct.MoveVector[Fx] << ", "
            << commandsStruct.MoveVector[Fy] << ", "
            << commandsStruct.MoveVector[Fz] << ", "
            << commandsStruct.MoveVector[Mx] << ", "
            << commandsStruct.MoveVector[My] << ", "
            << commandsStruct.MoveVector[Mz] << "]"
            << std::endl

            << "The Hand: ["
            << commandsStruct.TheHand[0] << ", "
            << commandsStruct.TheHand[1] << "]"
            << std::endl

            << "Camera: ["
            << commandsStruct.Camera[0] << ", "
            << commandsStruct.Camera[1] << "]"
            << std::endl

            << "MotorsLock: "
            << commandsStruct.MotorsLock << std::endl

            << "Stabilization: "
            << commandsStruct.Stabilization << std::endl;

    return ostream;
}