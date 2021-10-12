#include "./Core/Core.hpp"

using namespace Server;




Core::Core() : _socket(PORT){
    thread = std::thread(&Core::Run, this);
    thread.detach();
}



void Core::Run() {
    for (;;) {



    }
}
