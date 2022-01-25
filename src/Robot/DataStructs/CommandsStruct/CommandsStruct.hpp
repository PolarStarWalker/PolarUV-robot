#ifndef ROBOT_COMMANDSSTRUCT_HPP
#define ROBOT_COMMANDSSTRUCT_HPP

#include "./Math/StaticVector/StaticVector.hpp"
#include <iostream>

enum MoveVector : uint8_t {
    Fx = 0,
    Fy = 1,
    Fz = 2,
    Mx = 3,
    My = 4,
    Mz = 5
};

struct CommandsStruct {
    ///Array prototype
    ///{Fx, Fy, Fz, Mx, My, Mz}
    StaticVector<float, 6> MoveVector;
    StaticVector<float, 6> TheHand;
    StaticVector<float, 4> LowPWM;
    bool MotorsLock = true;
    bool Stabilization = false;
};

constexpr size_t CommandsStructLen = sizeof(CommandsStruct);
std::ostream &operator<<(std::ostream &ostream, const CommandsStruct &commandsStruct);

#endif
