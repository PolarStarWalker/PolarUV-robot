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

        std::shared_ptr<Service> sensors_;
        std::shared_ptr<RobotSettings> robotSettings_;
    };
}

#endif
