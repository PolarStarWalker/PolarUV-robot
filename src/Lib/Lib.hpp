#ifndef ROBOT_LIB_HPP
#define ROBOT_LIB_HPP

#include "./Math/Math.hpp"
#include "./Peripheral/Peripheral.hpp"
#include "./Coroutine/All.hpp"
#include "./DataTransmissions/DataTransmissions.hpp"

namespace lib {

    inline void LoggerInitialize(){
#if DEBUG
        std::clog.rdbuf(std::cout.rdbuf());
#else
        static std::fstream output("log.txt", std::ios_base::out | std::ios_base::trunc);
        std::clog.rdbuf(output.rdbuf());
#endif
    }

    static void Initialize() {
        LoggerInitialize();
        auto& network = network::TcpSession::GetInstance();
    }
}
#endif