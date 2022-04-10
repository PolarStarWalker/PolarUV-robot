#ifndef ROBOT_BASE_HPP
#define ROBOT_BASE_HPP

#include <string>
#include <TcpSession/Packet.hpp>

namespace lib::exceptions {

    struct BaseException : std::exception {
    public:
        BaseException(const std::string_view& message, lib::network::Response::CodeEnum code) :
                Message(message),
                Code(code){}

        const std::string_view Message;
        const lib::network::Response::CodeEnum Code;
    };

}
#endif