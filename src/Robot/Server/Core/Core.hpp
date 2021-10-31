#ifndef ROBOT_CORE_HPP
#define ROBOT_CORE_HPP

#include "../../DataTransmissions/Socket/Socket.hpp"
#include "../Message/Packet.hpp"
#include "../IRequest.hpp"

#include <shared_mutex>
#include <thread>
#include <queue>
#include <future>

///Port created by https://steamcommunity.com/profiles/76561198126802792/
#define PORT 32269

namespace Server {

    enum StatusCodeType : uint8_t {
        OK = 0,
        NotOk = 1,
    };

    struct Response {
        Response(StatusCodeType statusCode, const std::byte *data) :
                StatusCode(statusCode),
                Data(data) {}

        const StatusCodeType StatusCode;
        const std::byte *const Data;
    };

    struct Request {
        explicit Request(const Packet *packet, const std::promise<Response> &promise) :
                _packet(packet),
                _promise(&promise){}

        const Packet *_packet;
        const std::promise<Response> *_promise;
    };

    class Core {
    private:
        std::thread thread;

        std::queue<const Request *> _requestQueue;
        std::shared_mutex _requestQueueMutex;
        Socket _socket;

        [[noreturn]]
        void Run();

        explicit Core(uint16_t port);

        inline size_t GetQueueSize(){
            _requestQueueMutex.lock_shared();
            size_t size = _requestQueue.size();
            _requestQueueMutex.unlock_shared();

            return size;
        }

    public:

        static Core *GetInstance() {
            static Core core(PORT);
            return &core;
        }

        Response Request(const Packet *packet) {
            std::promise<Response> promise;
            std::future<Response> future = promise.get_future();

            struct Request request(packet, promise);

            _requestQueueMutex.lock();
            _requestQueue.push(&request);
            _requestQueueMutex.unlock();

            return future.get();
        }
    };
};

#endif