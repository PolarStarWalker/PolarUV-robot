#ifndef ROBOT_COMMANDSSTRUCT_HPP
#define ROBOT_COMMANDSSTRUCT_HPP

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
    float VectorArray[6] = {};
    float TheHand[2] = {};
    float Camera[2] = {};
    bool MotorsLock = true;
    bool Stabilization = false;
};

extern CommandsStruct CommandsStructData;
constexpr size_t CommandsStructLen = sizeof(CommandsStructData);

std::ostream &operator<<(std::ostream &ostream, const CommandsStruct &commandsStruct);

#endif
