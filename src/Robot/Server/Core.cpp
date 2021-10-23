#include "./Core/Core.hpp"

using namespace Server;

Core::Core(uint16_t port) : _socket(port){
    thread = std::thread(&Core::Run, this);
    thread.detach();
}


void Core::Run() {
    for (;;) {



    }
}
