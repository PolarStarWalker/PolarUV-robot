#ifndef ROBOT_BASE_HPP
#define ROBOT_BASE_HPP

#include <string>

namespace lib::exceptions {

    class BaseException : std::exception {
    public:
        BaseException(std::string_view header, std::string_view& message) :
                Header(header),
                Message(message){}

        const std::string_view Message;
        const std::string_view Header;
    };

}
#endif