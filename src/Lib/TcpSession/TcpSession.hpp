#ifndef ROBOT_TCPSESSION_HPP
#define ROBOT_TCPSESSION_HPP

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <Exceptions/Exceptions.hpp>
#include <chrono>
#include <boost/container/flat_map.hpp>
#include "Packet.hpp"

namespace lib::network {

    class TcpSession;

    struct IService {
        friend TcpSession;
        using ResponseBufferType = std::string;
    protected:

        using Response = lib::network::Response;

        explicit IService(ssize_t serviceId) :
                serviceId_(serviceId) {}

        virtual ~IService() = default;

    private:

        Response ReadData();

        Response WriteData(const std::string_view &data);

        Response WriteReadData(const std::string_view &data);

    protected:

        /*validate functions*/
        virtual bool ReadValidate();

        virtual bool WriteValidate(const std::string_view &data);

        virtual bool WriteReadValidate(const std::string_view &data);

        /*request function*/
        virtual ResponseBufferType Read();

        virtual void Write(const std::string_view &data);

        virtual ResponseBufferType WriteRead(const std::string_view &data);

        /*callbacks*/
        virtual void ConnectionLost();

    public:
        ///ToDo: когда-нибудь переехать на строки
        const uint64_t serviceId_;

    private:
        Response DoAction(RequestTypeEnum type, const std::string_view &data);
    };

    template<class Service>
    concept is_service = std::is_convertible_v<Service *, IService *> &&
                         !std::is_copy_constructible_v<Service> &&
                         !std::is_move_constructible_v<Service> &&
                         !std::is_copy_assignable_v<Service> &&
                         !std::is_move_assignable_v<Service>;

    class TcpSession {
        using IService_ptr = std::shared_ptr<IService>;
        using Map = boost::container::flat_map<size_t, IService_ptr>;

    public:
        constexpr static const size_t BUFFER_SIZE = 1024;
        constexpr static const uint16_t PORT = 2022;
        static constexpr auto CONNECTION_TIMEOUT =
#ifdef DEBUG
                std::chrono::milliseconds(200);
#else
                std::chrono::milliseconds(20);
#endif

        TcpSession() = default;

        ~TcpSession() = default;

        TcpSession(const TcpSession &) = delete;

        TcpSession(TcpSession &&) = delete;

        [[noreturn]]
        void Start();

    private:

        IService &FindService(ssize_t key);

        Map services_;

    public:

        template<is_service Service, typename... Args>
        std::shared_ptr<Service> CreateService(Args &&...args) {

            auto service = std::make_shared<Service>(std::forward<Args>(args)...);

            services_.insert({service->serviceId_, service});

            return service;
        }
    };
}
#endif
