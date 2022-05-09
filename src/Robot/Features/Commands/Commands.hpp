#ifndef ROBOT_COMMANDS_HPP
#define ROBOT_COMMANDS_HPP

#include <TcpSession/TcpSession.hpp>
#include "CommandsCycle.hpp"

namespace app {

    class CommandsService final : public lib::network::IService {
    public:
        CommandsService(ssize_t id, MotorsSender::IMotorsSender &motorsSender,
                        std::shared_ptr<Sensors> sensors,
                        std::shared_ptr<RobotSettings> settings);

        void Write(const std::string_view &data) final;

        CommandsService(const CommandsService &) = delete;

        CommandsService(CommandsService &&) = delete;

        CommandsService &operator=(const CommandsService &) = delete;

        CommandsService &operator=(CommandsService &&) = delete;

    private:

        CommandsCycle commandsCycle_;
    };

}

#endif
