#include "Lib.hpp"

#include <iostream>
#include <fstream>

void lib::LoggerInitialize(){
#if DEBUG
    std::clog.rdbuf(std::cout.rdbuf());
#else
    static std::fstream output("log.txt", std::ios_base::out | std::ios_base::trunc);
    std::clog.rdbuf(output.rdbuf());
#endif
}