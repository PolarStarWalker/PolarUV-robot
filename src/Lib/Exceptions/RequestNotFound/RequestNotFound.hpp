#ifndef ROBOT_REQUESTNOTFOUND_HPP
#define ROBOT_REQUESTNOTFOUND_HPP

#include "../Base/Base.hpp"

namespace lib::exceptions {

    struct NotFount : public BaseException {
    public:
        explicit NotFount(std::string_view message) :
                BaseException(message, network::Response::NotFound) {}
    };

}
#endif
