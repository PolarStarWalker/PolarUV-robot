#ifndef ROBOT_BASE_HPP
#define ROBOT_BASE_HPP

#include "DataTransmissions/TcpSession/Packet.hpp"
#include <string>

namespace lib::exceptions {

    class BaseException : std::exception {
    public:
        BaseException(std::string_view& message, network::Response::CodeEnum code) :
                Message(message),
                Code(code){}

        const std::string_view Message;
        const network::Response::CodeEnum Code;
    };

}
#endif