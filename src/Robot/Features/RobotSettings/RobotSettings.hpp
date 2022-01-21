#ifndef ROBOT_ROBOTSETTINGS_HPP
#define ROBOT_ROBOTSETTINGS_HPP

#include "./DataTransmissions/TcpSession/TcpSession.hpp"

namespace app {

    struct RobotSettingsData{
        float HandArray[6];
        float ThrustersArray[6 * 12];
        float LowTimer[4];
    };

    class RobotSettings final : public lib::network::IService {
    public:

        explicit RobotSettings(ssize_t id, std::string_view filename);

        void Validate() final;

        lib::network::Response Write(std::string_view &data) final;

    private:
        const std::string_view filename_;
    };

}
#endif