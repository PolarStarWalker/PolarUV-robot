#ifndef ROBOT_TCPSESSION_HPP
#define ROBOT_TCPSESSION_HPP

#include <boost/asio.hpp>
#include <unordered_map>
#include "./Packet.hpp"

#include "../../ThreadPool/ThreadPool.hpp"


namespace lib::network {

    class IService;

    class TcpSession {
        friend IService;
    public:
        TcpSession(const TcpSession &) = delete;

        TcpSession(TcpSession &&) = delete;

        static TcpSession &GetInstance();

        [[noreturn]]
        void Start();

        constexpr static const size_t PORT = 2022;
        constexpr static size_t BUFFER_SIZE = 1024;
    private:
        TcpSession();

        ~TcpSession();

        void AddService(std::shared_ptr<IService>&&);

        std::unordered_map<size_t, std::shared_ptr<IService>> services_;

        boost::asio::io_context _ioContext;

        std::thread _thread;
    };

    struct IService : std::enable_shared_from_this<IService>{
        using Response = lib::network::Response;

        explicit IService(size_t serviceId) :
                _serviceId(serviceId) {}

        virtual void Validate() = 0;

        virtual Response Read(std::string_view data);

        virtual Response Write(std::string_view data);

        virtual Response ReadWrite(std::string_view data);

        const size_t _serviceId;

        template <class Service, typename... Args>
        static void RegisterService(Args&&...args){

            auto service = std::make_shared<Service>(args...);

            TcpSession::GetInstance().AddService(service);
        };
    };
}
#endif
