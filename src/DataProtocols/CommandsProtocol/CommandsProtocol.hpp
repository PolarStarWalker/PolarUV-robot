#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP
#include <shared_mutex>
#include <thread>
#include <cstring>


#include "../Socket/Socket.hpp"
#include "../SPI/SPI.hpp"
#include "../../DataStructs/DataStructs.hpp"
#include "../../Services/SettingsFileService/SettingsFileService.hpp"

class CommandsProtocol{
public:
    CommandsProtocol(char* SPIDevice);

    void Start();
    void StartAsync();
    void SetSettingsStruct();

private:
    SettingsStruct _settings;
    Socket _socket;
    SPI _spi;
    std::thread _protocolThread;

    std::shared_mutex _settingsMutex;


};


#endif
