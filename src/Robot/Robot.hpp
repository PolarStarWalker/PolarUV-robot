#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "./DataProtocols/DataProtocols.hpp"
#include "./Peripheral/Peripheral.hpp"

class Robot {

public:
    Robot();

    void TNPA();


private:
    DataProtocols::CommandsProtocol commandsProtocol;
};


#endif
