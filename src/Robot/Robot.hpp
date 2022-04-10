#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "./Features/Features.hpp"
#include "./Peripheral/Peripheral.hpp"

namespace app {

    class Robot {
    public:
        Robot();

        void SetUp();

        void Start();

    private:

        lib::network::TcpSession network_;
        StartSettings startSettings_;
        MotorsSender::IMotorsSender &motorsSender_;
        std::shared_ptr<Sensors> sensors_;
        std::shared_ptr<RobotSettings> robotSettings_;
        std::shared_ptr<Video> video_;
        std::shared_ptr<CommandsService> commandsService_;
    };
}

#endif
