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

        TcpSession()= default;

        ~TcpSession() = default;

        IService& FindService(const RequestHeaderType&);
        void AddService(std::unique_ptr<IService>&&);

        std::unordered_map<size_t, std::unique_ptr<IService>> services_;

        std::thread thread_;
    };

    struct IService : std::enable_shared_from_this<IService>{
        using Response = lib::network::Response;

        explicit IService(ssize_t serviceId) :
                serviceId_(serviceId) {}

        virtual void Validate() = 0;

        virtual Response Read(std::string_view &data);

        virtual Response Write(std::string_view &data);

        virtual Response ReadWrite(std::string_view &data);

        const ssize_t serviceId_;

        template <class Service, typename... Args>
        static void RegisterService(Args&&...args){

            auto service = std::make_unique<Service>(args...);

            TcpSession::GetInstance().AddService(std::move(service));
        };
    };
}
#endif
