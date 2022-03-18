#ifndef ROBOT_COMMANDS_HPP
#define ROBOT_COMMANDS_HPP

#include <DataTransmissions/TcpSession/TcpSession.hpp>
#include <Math/Math.hpp>
#include "../Sensors/Sensors.hpp"
#include "../RobotSettings/RobotSettings.hpp"
#include "../MotorsSender/IMotorsSender.hpp"

namespace app {

    struct CommandsStruct {
        enum MoveDimensions : uint8_t {
            Fx = 0,
            Fy = 1,
            Fz = 2,
            Mx = 3,
            My = 4,
            Mz = 5
        };

        enum StabilizationType : int8_t {
            None = 0,
            Rotation = 1,
            Absolute = 2
        };

        stc::VerticalVector<float, 6> Move{};
        std::array<float, 6> Hand{};
        std::array<float, 4> LowPWM{};

        StabilizationType Stabilization = None;

        friend std::ostream &operator<<(std::ostream &output, const CommandsStruct &commands) {
            output << "[COMMANDS STRUCT]\n"
                   << "Move vector: "
                   << commands.Move[0] << ", "
                   << commands.Move[1] << ", "
                   << commands.Move[2] << ", "
                   << commands.Move[3] << ", "
                   << commands.Move[4] << ", "
                   << commands.Move[5] << ", "
                   << '\n'
                    << "Hand vector: "
                   << commands.Hand[0] << ", "
                   << commands.Hand[1] << ", "
                   << commands.Hand[2] << ", "
                   << commands.Hand[3] << ", "
                   << commands.Hand[4] << ", "
                   << commands.Hand[5] << ", "
                   << '\n'
                    << "LowPWM vector: "
                   << commands.LowPWM[0] << ", "
                   << commands.LowPWM[1] << ", "
                   << commands.LowPWM[2] << ", "
                   << commands.LowPWM[3];

            return output;
        }
    };

    class CommandsService final : public lib::network::IService {
    public:
        CommandsService(ssize_t id, MotorsSender::IMotorsSender *motorsSender, std::shared_ptr<Sensors> sensors,
                        std::shared_ptr<RobotSettings> settings);

        Response Write(std::string_view &data) final;

        CommandsService(const CommandsService &) = delete;

        CommandsService(CommandsService &&) = delete;

        CommandsService &operator=(const CommandsService &) = delete;

        CommandsService &operator=(CommandsService &&) = delete;

    private:

        std::shared_ptr<Sensors> sensors_;
        std::shared_ptr<RobotSettings> settings_;
        MotorsSender::IMotorsSender *motorsSender_;
    };

}

#endif
