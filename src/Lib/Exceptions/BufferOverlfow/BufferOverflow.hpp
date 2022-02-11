#ifndef ROBOT_BUFFEROVERFLOW_HPP
#define ROBOT_BUFFEROVERFLOW_HPP

#include "../Base/Base.hpp"


namespace lib::exceptions {
    struct BufferOverflow : public BaseException {
    public:
        explicit BufferOverflow(std::string_view message) :
                BaseException(message, network::Response::BufferOverflow) {}
    };

}

#endif //ROBOT_BUFFEROVERFLOW_HPP
