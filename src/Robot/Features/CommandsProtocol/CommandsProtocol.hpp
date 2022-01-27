#ifndef ROBOT_COMMANDSPROTOCOL_HPP
#define ROBOT_COMMANDSPROTOCOL_HPP

#include <shared_mutex>
#include <thread>
#include <cstring>

#include "../RobotSettingsProtocol/RobotSettingsProtocol.hpp"
#include "../MotorsSender/IMotorsSender.hpp"
#include "../CommandsReceiver/ICommandsReceiver.hpp"
#include "../../DataStructs/DataStructs.hpp"

#include "Peripheral/PeripheralHandler/PeripheralHandler.hpp"
#include "../Sensors/Sensors.hpp"

namespace DataProtocols {

    class CommandsProtocol {

    public:

        CommandsProtocol(const MotorsSender::IMotorsSender &motorsSender,
                         const CommandsReceiver::ICommandsReceiver &commandsReceiver,
                         std::shared_ptr<app::Sensors> sensors);

        [[noreturn]]
        void Start();

    private:
        //Socket _commandsSocket;
        const CommandsReceiver::ICommandsReceiver& _commandsReceiver;
        const MotorsSender::IMotorsSender& _motorsSender;
        const std::shared_ptr<app::Sensors> sensors_;
    };
}

#endif
