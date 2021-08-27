#include "./CommandsStruct/CommandsStruct.hpp"

std::ostream& operator<<(std::ostream &ostream, const CommandsStruct &commandsStruct) {
    ostream << "VectorArray: ["
            << commandsStruct.VectorArray[Fx] << ", "
            << commandsStruct.VectorArray[Fy] << ", "
            << commandsStruct.VectorArray[Fz] << ", "
            << commandsStruct.VectorArray[Mx] << ", "
            << commandsStruct.VectorArray[My] << ", "
            << commandsStruct.VectorArray[Mz] << "]"
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