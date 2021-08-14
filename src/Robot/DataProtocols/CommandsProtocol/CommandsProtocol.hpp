#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP

#include <shared_mutex>
#include <thread>
#include <cstring>

#include "../Matrix/Matrix.hpp"
#include "../DataTransmissions/DataTransmissions.hpp"
#include "../RobotSettingsProtocol/RobotSettingsProtocol.hpp"
#include "../DataStructs/DataStructs.hpp"

namespace DataProtocols {

    class CommandsProtocol {
    public:
        explicit CommandsProtocol(const char *SPIDevice);

        void Start();

    private:
        Socket _commandsSocket;
        SPI _spi;
        std::fstream file;
    };
}

#endif
