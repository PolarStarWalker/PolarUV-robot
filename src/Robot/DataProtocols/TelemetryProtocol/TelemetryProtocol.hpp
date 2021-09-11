#ifndef ROBOT_TELEMETRYPROTOCOL_HPP
#define ROBOT_TELEMETRYPROTOCOL_HPP
#include "../BaseProtocol/BaseProtocol.hpp"
#include "../../DataTransmissions/Socket/Socket.hpp"
#include "../../DataStructs/DataStructs.hpp"

class TelemetryProtocol : protected BaseProtocol{
public:
    TelemetryProtocol();

    void Start();
    void StartAsync();

    inline void SetTelemetryStruct(const TelemetryStruct& telemetryStruct){
        this->_telemetryMutex.lock();
        this->_telemetryStruct = telemetryStruct;
        this->_telemetryMutex.unlock();
    }

    inline TelemetryStruct GetTelemetryStruct(){
        this->_telemetryMutex.lock_shared();
        TelemetryStruct data = this->_telemetryStruct;
        this->_telemetryMutex.unlock_shared();
        return data;
    }

private:
    Socket _socket;
    TelemetryStruct _telemetryStruct;
    std::shared_mutex _telemetryMutex;
};

#endif
