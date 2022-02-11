#ifndef ROBOT_STARTSETTINGS_HPP
#define ROBOT_STARTSETTINGS_HPP

#include <fstream>
#include "../MotorsSender/IMotorsSender.hpp"
#include "../MotorsSender/SPI.hpp"

#include "../CommandsReceiver/ICommandsReceiver.hpp"
#include "../CommandsReceiver/Network.hpp"

#include "Math/SIPrefix.hpp"

class StartSettings {
public:

    static StartSettings Get() {
        StartSettings settings{};
        std::fstream file("StartSettings", std::ios_base::in | std::ios_base::binary);
        file.read((char *) &settings, sizeof(settings));
        return settings;
    }

    MotorsSender::IMotorsSender &GetMotorsSender() {
        switch (_motorsSenderId) {
            case MotorsSender::SPI:
                static class MotorsSender::SPI spi( "/dev/spidev0.0", Mega(35));
                return spi;
        }
        std::terminate();
    }

    CommandsReceiver::ICommandsReceiver &GetCommandsReceiver() {
        switch (_commandsReceiverId) {
            case CommandsReceiver::Net:
                static class CommandsReceiver::Network net(1999);
                return net;
        }
        std::terminate();
    }

private:

    StartSettings() : _motorsSenderId(MotorsSender::SPI), _commandsReceiverId(CommandsReceiver::Net){}

    CommandsReceiver::Id _commandsReceiverId;
    MotorsSender::Id _motorsSenderId;
};


#endif
