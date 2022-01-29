#ifndef ROBOT_COMMANDS_HPP
#define ROBOT_COMMANDS_HPP

#include <DataTransmissions/TcpSession/TcpSession.hpp>
#include <Math/Math.hpp>
#include "../Sensors/Sensors.hpp"
#include "../RobotSettings/RobotSettings.hpp"

namespace app {

    struct CommandsStruct {
        StaticVector<float, 6> Move{};
        StaticVector<float, 6> Hand{};
        StaticVector<float, 4> LowPWM{};
    };

    class CommandsService final : public lib::network::IService {
    public:
        CommandsService(ssize_t id, std::shared_ptr<Sensors> sensors, std::shared_ptr<RobotSettings> settings);

        Response WriteRead(std::string_view &data) final;

        CommandsService(const CommandsService&) = delete;
        CommandsService(CommandsService&&) = delete;

        CommandsService& operator=(const CommandsService&) = delete;
        CommandsService& operator=(CommandsService&&) = delete;

    private:

        std::shared_ptr<Sensors> sensors_;
        std::shared_ptr<RobotSettings> settings_;
    };

}

#endif
