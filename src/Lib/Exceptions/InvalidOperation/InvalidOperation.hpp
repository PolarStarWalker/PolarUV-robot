#ifndef ROBOT_INVALIDOPERATION_HPP
#define ROBOT_INVALIDOPERATION_HPP

#include "../Base/Base.hpp"

namespace lib::exceptions {

    struct InvalidOperation : public BaseException {
    public:
        explicit InvalidOperation(std::string_view message) :
                BaseException(message, network::Response::BadRequest) {}
    };

}

#endif
