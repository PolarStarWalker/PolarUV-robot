#ifndef ROBOT_NETWORK_HPP
#define ROBOT_NETWORK_HPP

#include "./ICommandsSender.hpp"
#include "../../DataTransmissions/Socket/Socket.hpp"

namespace CommandsReceiver {

    class Network final : public ICommandsSender {
    public:
        explicit Network(uint16_t port) : _socket(port) {}

        CommandsStruct GetCommandsStruct() const final {
            CommandsStruct commandsStruct;
            _socket.RecvDataLen((char *) &commandsStruct, CommandsStructLen);
            return commandsStruct;
        }

        void SendTelemetryStruct(const TelemetryStruct &telemetry) const final {
            _socket.SendDataLen((char *) &telemetry, TelemetryStructLen);
        }

        void Wait() const final { _socket.Listen(); }

        bool IsOnline() const final { return _socket.IsOnline(); };

    private:
        Socket _socket;
    };
}
#endif