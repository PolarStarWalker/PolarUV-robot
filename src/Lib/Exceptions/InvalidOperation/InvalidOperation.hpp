#ifndef ROBOT_INVALIDOPERATION_HPP
#define ROBOT_INVALIDOPERATION_HPP

#include "../Base/Base.hpp"

namespace lib::exceptions {

    class InvalidOperation : public BaseException {
    public:
        explicit InvalidOperation(std::string_view message) :
                BaseException("Неправильная операция", message) {}
    };

}

#endif
