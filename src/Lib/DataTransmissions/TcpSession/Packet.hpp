#ifndef ROBOT_PACKET_HPP
#define ROBOT_PACKET_HPP

#include <variant>

namespace lib::network {

    enum class TypeEnum : size_t {
        R,
        W,
        RW
    };

    struct RequestHeaderType {
        TypeEnum Type{};
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

        explicit operator bool() const { return Header.Code == Ok || Header.Code == NoContent; }
    };


}
#endif