#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP

#include <shared_mutex>
#include <thread>
#include <cstring>

#include "../RobotSettingsProtocol/RobotSettingsProtocol.hpp"
#include "../MotorsSender/IMotorsSender.hpp"
#include "../CommandsSender/ICommandsSender.hpp"
//#include "../../DataTransmissions/DataTransmissions.hpp"
#include "../../DataStructs/DataStructs.hpp"
#include "../../Peripheral/PeripheralHandler/PeripheralHandler.hpp"


namespace DataProtocols {

    class CommandsProtocol {

    public:



        CommandsProtocol(const MotorsSender::IMotorsSender& motorsSender,
                                  const CommandsReceiver::ICommandsSender& commandsSender,
                                  const PeripheralHandler& peripheralHandler);

        [[noreturn]]
        void Start();

    private:
        //Socket _commandsSocket;
        const CommandsReceiver::ICommandsSender& _commandsSender;
        const MotorsSender::IMotorsSender& _motorsSender;
        const PeripheralHandler& _peripheralHandler;
    };
}

#endif
