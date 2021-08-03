#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP
#include <shared_mutex>
#include <thread>
#include <cstring>


#include "../DataTransmissions/DataTransmissions.hpp"
#include "../../DataStructs/DataStructs.hpp"
#include "../../Services/SettingsFileService/SettingsFileService.hpp"

class CommandsProtocol{
public:
    explicit CommandsProtocol(char* SPIDevice);

    void Start();
    void StartAsync();

private:
    Socket _commandsSocket;
    SPI _spi;
    std::thread _protocolThread;

};


#endif
