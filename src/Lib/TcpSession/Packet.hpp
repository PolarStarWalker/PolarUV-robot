#ifndef ROBOT_PACKET_HPP
#define ROBOT_PACKET_HPP

#include <cstddef>
#include <sys/types.h>
#include <string>

namespace lib::network {

    struct Request {
    private:
        char *data_;
        size_t size_;

    public:
        template<typename Type>
        Type* As(){

            if(size_ == sizeof(Type))
                return (Type*) data_;

            return nullptr;
        }

        char* begin(){return data_;}

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
                Data(std::move(data)),
                Header(code, endpointId, data.size()) {};

        Response(Response &&response) noexcept:
                Data(std::move(response.Data)),
                Header(response.Header) {}


        template<typename Returned, class Obj, typename ... Args>
        static std::string GetData(Returned (Obj::*func)(Args...) const, const Obj* obj, Args&& ... args){

            std::string data(sizeof(Returned), 0);

            auto& typedData = (Returned&) *data.c_str();

            typedData = (obj->*func)(std::forward(args)...);

            return data;
        }

        explicit operator bool() const { return Header.Code == Ok || Header.Code == NoContent; }
    };


}
#endif