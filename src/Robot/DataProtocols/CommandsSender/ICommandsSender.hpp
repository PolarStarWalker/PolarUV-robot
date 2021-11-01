#ifndef ROBOT_ICOMMANDSSENDER_HPP
#define ROBOT_ICOMMANDSSENDER_HPP

#include "../../DataStructs/DataStructs.hpp"

class ICommandsSender{
public:
    virtual CommandsStruct GetCommandsStruct() const = 0;
    virtual void SendTelemetryStruct(const TelemetryStruct& telemetry) const = 0;
    virtual bool IsOnline() const = 0;
    virtual void Wait() const = 0;
};

#endif