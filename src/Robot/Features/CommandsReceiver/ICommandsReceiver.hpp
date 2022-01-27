#ifndef ROBOT_ICOMMANDSRECEIVER_HPP
#define ROBOT_ICOMMANDSRECEIVER_HPP

#include "../../DataStructs/DataStructs.hpp"
#include "../Sensors/Sensors.hpp"

namespace CommandsReceiver {

    enum Id : size_t {
        Net = 0,
    };

    class ICommandsReceiver {
    public:

        virtual CommandsStruct GetCommandsStruct() const = 0;

        virtual void SendTelemetryStruct(const app::SensorsStruct &telemetry) const = 0;

        virtual bool IsOnline() const = 0;

        virtual void Wait() const = 0;
    };
}
#endif