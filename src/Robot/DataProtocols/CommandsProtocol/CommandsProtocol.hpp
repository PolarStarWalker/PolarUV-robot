#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP

#include <shared_mutex>
#include <thread>
#include <cstring>

#include "../RobotSettingsProtocol/RobotSettingsProtocol.hpp"
#include "../../DataTransmissions/DataTransmissions.hpp"
#include "../../DataStructs/DataStructs.hpp"

namespace DataProtocols {

    class CommandsProtocol {
    public:
        explicit CommandsProtocol(const char *SPIDevice, uint32_t speed_hz, size_t peripheralTimeout_us);

        [[noreturn]]
        void Start();

    private:
        Socket _commandsSocket;
        std::atomic<CommandsStruct> _commandsStruct;
        SPI _spi;
        const size_t _peripheralTimeout_us;
    };
}

#endif
