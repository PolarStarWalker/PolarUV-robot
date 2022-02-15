#ifndef ROBOT_TCPSESSION_HPP
#define ROBOT_TCPSESSION_HPP

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <Exceptions/Exceptions.hpp>
#include "Packet.hpp"


namespace lib::network {

    class IService;

    class TcpSession {
        friend IService;
        using Map = std::unordered_map<size_t, std::shared_ptr<IService>>;

    public:
        TcpSession(const TcpSession &) = delete;

        TcpSession(TcpSession &&) = delete;

        static TcpSession &GetInstance();

        [[noreturn]]
        void Start();

        constexpr static const size_t BUFFER_SIZE = 1024;
        constexpr static const uint16_t PORT = 2022;

    private:

        TcpSession() = default;

        ~TcpSession() = default;

        IService &FindService(ssize_t key);

        Map services_;
    };


    template<class Service>
    concept is_service = std::is_convertible_v<Service *, IService *> &&
                         !std::is_copy_constructible_v<Service> &&
                         !std::is_move_constructible_v<Service> &&
                         !std::is_copy_assignable_v<Service> &&
                         !std::is_move_assignable_v<Service>;

    struct IService : std::enable_shared_from_this<IService> {
        friend TcpSession;
    protected:

        using Response = lib::network::Response;

        explicit IService(ssize_t serviceId) :
                serviceId_(serviceId) {}

    private:

        Response ReadData(std::string_view &data);

        Response WriteData(std::string_view &data);

        Response WriteReadData(std::string_view &data);

    protected:

        virtual bool ReadValidate(std::string_view &data);

        virtual bool WriteValidate(std::string_view &data);

        virtual bool WriteReadValidate(std::string_view &data);

        virtual Response Read(std::string_view &data);

        virtual Response Write(std::string_view &data);

        virtual Response WriteRead(std::string_view &data);

        ///ToDo: когда-нибудь переехать на строки
        const ssize_t serviceId_;

    public:

        template<is_service Service, typename... Args>
        static std::shared_ptr<Service> RegisterService(Args &&...args) {

            auto service = std::make_shared<Service>(args...);

            TcpSession::GetInstance().services_[service->serviceId_] = service;

            return service;
        };


    private:
        template<size_t buffer_size>
        Response DoAction(const RequestHeaderType &header, const std::array<char, buffer_size> &dataBuffer) {

            std::string_view data(dataBuffer.data(), header.Length);

            switch (header.Type) {
                case TypeEnum::R:
                    return this->ReadData(data);

                case TypeEnum::W:
                    return this->WriteData(data);

                case TypeEnum::RW:
                    return this->WriteReadData(data);

                default:
                    throw lib::exceptions::NotFount("Такой типа запроса не определён библиотекой");
            }
        }
    };
}
#endif
