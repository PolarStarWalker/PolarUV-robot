#ifndef ROBOT_CORE_HPP
#define ROBOT_CORE_HPP
#include "../../DataTransmissions/Socket/Socket.hpp"
#include <shared_mutex>
#include <thread>

///Port created by https://steamcommunity.com/profiles/76561198126802792/
#define PORT 32269

namespace Server {

    /*struct Message{
        std::string
    };
*/

    struct Header{

    };

    class Core {
    private:

        std::thread thread;

        Socket _socket;

        [[noreturn]]
        void Run();

        Core();


    public:
        static Core *GetInstance() {
            static Core core;
            return &core;
        }

        std::string Read();
    };
};

#endif