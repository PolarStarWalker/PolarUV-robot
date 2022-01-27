#ifndef ROBOT_COMMANDS_HPP
#define ROBOT_COMMANDS_HPP

#include <DataTransmissions/TcpSession/TcpSession.hpp>
#include <Math/Math.hpp>
#include "../Sensors/Sensors.hpp"

namespace app {
    struct CommandsStruct {
        StaticVector<float, 6> Move{};
        StaticVector<float, 6> Hand{};
        StaticVector<float, 4> LowPWM{};
    };


    class CommandsService final : public lib::network::IService {
    public:
        explicit CommandsService(ssize_t id);

        Response WriteRead(std::string_view &data) final;

    private:

        std::shared_ptr<Sensors> sensors_;
    };

}

#endif
