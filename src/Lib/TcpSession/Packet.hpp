#ifndef ROBOT_PACKET_HPP
#define ROBOT_PACKET_HPP

#include <cstddef>
#include <sys/types.h>
#include <string>
#include <ostream>

namespace lib::network {

    struct Request {
    private:
        char *data_;
        size_t size_;

    public:
        template<typename Type>
        Type *As() {

            if (size_ == sizeof(Type))
                return (Type *) data_;

            return nullptr;
        }

        char *begin() { return data_; }

    };

    enum class RequestTypeEnum : size_t {
        R,
        W,
        RW
    };

    struct RequestHeaderType {
        RequestTypeEnum Type{};
        ssize_t EndpointId{};
        size_t Length{};
    };

    inline std::ostream &operator<<(std::ostream &out, const RequestHeaderType &header) {

        out << "[REQUEST HEADER]\n"
            << "Endpoint: " << header.EndpointId << ", Type: ";

        switch (header.Type) {

            case RequestTypeEnum::R:
                out << 'R';
                break;
            case RequestTypeEnum::W:
                out << 'W';
                break;
            case RequestTypeEnum::RW:
                out << "RW";
                break;
            default:
                out << "UNDEFINED";
        }

        out << ", Data length: " << header.Length;

        return out;
    }

    struct Response {

        enum CodeEnum : size_t {
            Ok,
            NoContent,
            BadRequest,
            ConnectionError,
            BufferOverflow,
            ConnectionTimeout,
            NotFound,
        };

        struct HeaderType {
            CodeEnum Code{};
            ssize_t EndpointId{};
            size_t Length{};

            HeaderType(CodeEnum code, ssize_t endpoint, size_t length) :
                    Code(code),
                    EndpointId(endpoint),
                    Length(length) {}

            HeaderType() = default;
        };

    public:

        const HeaderType Header;
        std::string Data;

        Response(std::string &&data, enum CodeEnum code, ssize_t endpointId) :
                Header(code, endpointId, data.size()),
                Data(std::move(data)){};

        Response(Response &&response) noexcept:
                Header(response.Header),
                Data(std::move(response.Data)){}


        template<typename Returned, class Obj, typename ... Args>
        static std::string GetData(Returned (Obj::*func)(Args...) const, const Obj *obj, Args &&... args) {

            std::string data(sizeof(Returned), 0);

            auto &typedData = (Returned &) *data.c_str();

            typedData = (obj->*func)(std::forward(args)...);

            return data;
        }

        explicit operator bool() const { return Header.Code == Ok || Header.Code == NoContent; }
    };


}
#endif