#ifndef ROBOT_EXCEPTION_HPP
#define ROBOT_EXCEPTION_HPP

#include "../Base/Base.hpp"

namespace lib::exceptions {

    struct TransferError : public BaseException {
    public:
        explicit TransferError(std::string_view message) :
                BaseException( message, network::Response::ConnectionError) {}
    };

}

#endif