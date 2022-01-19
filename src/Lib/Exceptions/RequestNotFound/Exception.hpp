#ifndef ROBOT_EXCEPTION_HPP
#define ROBOT_EXCEPTION_HPP

#include "../Base/Base.hpp"

namespace lib::exceptions {

    class RequestNotFount : public BaseException {
    public:
        explicit RequestNotFount(std::string_view message) :
                BaseException("Такой запрос не найден", message) {}
    };

}
#endif
