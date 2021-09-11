#include "./TelemetryProtocol/TelemetryProtocol.hpp"

TelemetryProtocol::TelemetryProtocol() {
    this->_socket.MakeServerSocket(35864);
}

void TelemetryProtocol::Start() {

    for(;;){
        this->_socket.IsOnline();
        TelemetryStruct data = GetTelemetryStruct();
        _socket.RecvDataLen((char*) &data, TelemetryStructLen);
    }
}

void TelemetryProtocol::StartAsync() {
    std::thread telemetryThread(&TelemetryProtocol::Start, this);
}
