#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP

#include <shared_mutex>
#include <thread>
#include <cstring>

#include "../RobotSettingsProtocol/RobotSettingsProtocol.hpp"
#include "../../Math/Math.hpp"
#include "../../DataTransmissions/DataTransmissions.hpp"
#include "../../DataStructs/DataStructs.hpp"

namespace DataProtocols {

    class CommandsProtocol {
    public:
        explicit CommandsProtocol(const char *SPIDevice, uint32_t speed);

        [[noreturn]]
        void Start();

    private:
        Socket _commandsSocket;
        SPI _spi;
        std::fstream file;
    };
}

#endif
