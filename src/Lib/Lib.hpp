#ifndef ROBOT_LIB_HPP
#define ROBOT_LIB_HPP

#include "./Math/Math.hpp"
#include "./Peripheral/Peripheral.hpp"
#include "./Coroutine/All.hpp"
#include "./DataTransmissions/DataTransmissions.hpp"

namespace lib {

    void LoggerInitialize();

    static void Initialize() {
        LoggerInitialize();
        auto& network = network::TcpSession::GetInstance();
    }
}
#endif