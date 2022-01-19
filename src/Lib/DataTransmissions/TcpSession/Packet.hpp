#ifndef ROBOT_PACKET_HPP
#define ROBOT_PACKET_HPP

namespace lib::network {
    struct Response {
        enum CodeEnum : size_t {
            Ok,
            NoContent,
            BadRequest,
            ConnectionError,
            BufferOverflow,
            MethodNotFound,
        };

        std::string Data;
        const CodeEnum Code;

        Response(std::string &&data, enum CodeEnum code) : Data(std::move(data)), Code(code) {};

        explicit operator bool() const { return Code == Ok || Code == NoContent; }
    };

    enum class TypeEnum : size_t {
        R,
        W,
        RW
    };

    struct HeaderType {
        TypeEnum Type{};
        ssize_t EndpointId{};
        size_t Length{};
    };
}
#endif