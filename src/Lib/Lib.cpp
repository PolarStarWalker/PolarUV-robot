#include "Lib.hpp"

#include <iostream>
#include <fstream>

void LoggerInitialize() {
#if DEBUG
    std::clog.rdbuf(std::cout.rdbuf());
#else
    //static std::fstream output("log.txt", std::ios_base::out | std::ios_base::trunc);
    //std::clog.rdbuf(output.rdbuf());
    std::clog.rdbuf(std::cout.rdbuf());
#endif
}



void lib::Initialize() {
    LoggerInitialize();
    //InitGstreamer();
}