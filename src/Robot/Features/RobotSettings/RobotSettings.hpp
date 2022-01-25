#ifndef ROBOT_ROBOTSETTINGS_HPP
#define ROBOT_ROBOTSETTINGS_HPP
#include "./Math/StaticVector/StaticVector.hpp"
#include "./Math/StaticMatrix.hpp"
#include "./DataTransmissions/TcpSession/TcpSession.hpp"

namespace app {

    struct RobotSettingsData{
        StaticMatrix<float, 12, 6> ThrustersCoefficientArray;
        StaticVector<float, 6> HandCoefficientArray;
        size_t ThrustersNumber = 0;
        size_t HandFreedom = 0;
    };

    class RobotSettings final : public lib::network::IService {
        using Response = lib::network::Response;
    public:

        RobotSettings(const RobotSettings&) = delete;
        RobotSettings(RobotSettings&&) = delete;
        RobotSettings& operator=(const RobotSettings&) = delete;
        RobotSettings& operator=(RobotSettings&&) = delete;

        RobotSettings(ssize_t id, std::string_view filename);

        bool WriteValidate(std::string_view &robotSettings) final;

        Response Write(std::string_view &robotSettings) final;
        Response Read(std::string_view &request) final;

        static RobotSettingsData GetSettings();

    };

}
#endif