#ifndef ROBOT_TCPSESSION_HPP
#define ROBOT_TCPSESSION_HPP

#include <boost/asio.hpp>
#include <unordered_map>
#include "Packet.hpp"

//#include "../../ThreadPool/ThreadPool.hpp"


namespace lib::network {

    class IService;

    class TcpSession {
        friend IService;
        using Map = std::unordered_map<size_t, std::unique_ptr<IService>>;

    public:
        TcpSession(const TcpSession &) = delete;

        TcpSession(TcpSession &&) = delete;

        static TcpSession &GetInstance();

        [[noreturn]]
        void Start();

        constexpr static const size_t BUFFER_SIZE = 1024;
        constexpr static const uint16_t PORT = 2022;

    private:

        TcpSession()= default;

        ~TcpSession() = default;

        IService& FindService(const RequestHeaderType&);

        Map services_;

        std::thread thread_;
    };

    struct IService : std::enable_shared_from_this<IService>{
        using Response = lib::network::Response;

        explicit IService(ssize_t serviceId) :
                serviceId_(serviceId) {}

        virtual bool ReadValidate(std::string_view &data);
        virtual bool WriteValidate(std::string_view &data);

        virtual Response Read(std::string_view &data);

        virtual Response Write(std::string_view &data);

        virtual Response ReadWrite(std::string_view &data);

        ///ToDo: когда-нибудь переехать на строки
        const ssize_t serviceId_;

        template <class Service, typename... Args>
        static void RegisterService(Args&&...args){

            auto service = std::make_unique<Service>(args...);

            TcpSession::GetInstance().services_[service->serviceId_] = std::move(service);
        };
    };
}
#endif
