
#ifndef ROBOT_ROBOTVIDEOPROTOCOL_HPP
#define ROBOT_ROBOTVIDEOPROTOCOL_HPP

#include <fstream>
#include "unistd.h"
#include "../DataTransmissions/Socket/Socket.hpp"

class RobotVideoProtocol {
private:
    Socket _socket;
    pid_t childPid;
    void Start();

public:
    RobotVideoProtocol();
    void StartAsync();
    void StartPipeline();
};


#endif
