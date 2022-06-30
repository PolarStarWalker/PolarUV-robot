#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "./Features/Features.hpp"
#include "./Peripheral/Peripheral.hpp"

namespace app {

    class Robot {
    public:
        Robot();

        void SetUp();

        [[noreturn]]
        void Start();

    private:

        lib::network::TcpSession network_;
        StartSettings startSettings_;

        std::vector<std::shared_ptr<lib::network::IService>> services_;
    };
}

#endif
