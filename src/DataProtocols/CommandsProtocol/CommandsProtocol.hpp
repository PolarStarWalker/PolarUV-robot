#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP
#include <shared_mutex>
#include <thread>
#include <cstring>


#include "../Socket/Socket.hpp"
#include "../SPI/SPI.hpp"
#include "../../DataStruct.hpp"



class CommandsProtocol{
public:
    CommandsProtocol(char* SPIDevice);

private:
    Socket _socket;
    SPI _spi;

};


#endif
