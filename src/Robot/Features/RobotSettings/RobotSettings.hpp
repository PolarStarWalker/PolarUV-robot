#ifndef ROBOT_ROBOTSETTINGS_HPP
#define ROBOT_ROBOTSETTINGS_HPP

#include "./DataTransmissions/TcpSession/TcpSession.hpp"

namespace Application {

    struct RobotSettingsData{
        float HandArray[6];
        float ThrustersArray[6*12];
        float LowTimer[4];
    };

    class RobotSettings final : public lib::network::IService {

    public:

        explicit RobotSettings(size_t id);

        void Validate() final;

        lib::network::Response Write(std::string_view data) override final;
    };

}
#endif