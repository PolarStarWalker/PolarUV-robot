
#ifndef ROBOT_ROBOTVIDEOPROTOCOL_HPP
#define ROBOT_ROBOTVIDEOPROTOCOL_HPP

#include <fstream>
#include <unistd.h>
#include "../../DataTransmissions/Socket/Socket.hpp"
namespace DataProtocols {
    class RobotVideoProtocol {
    private:
        Socket _socket;
        pid_t childPid;

        void KillStream();

        [[noreturn]]
        void Start();

    public:
        RobotVideoProtocol();

        ~RobotVideoProtocol();

        void StartAsync();
    };

}
#endif
