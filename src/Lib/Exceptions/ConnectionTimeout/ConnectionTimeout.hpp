#ifndef ROBOT_CONNECTIONTIMEOUT_HPP
#define ROBOT_CONNECTIONTIMEOUT_HPP

#include "../Base/Base.hpp"


namespace lib::exceptions {
    struct ConnectionTimeout : public BaseException {
    public:
        explicit ConnectionTimeout() :
                BaseException("[CONNECTION TIMEOUT]", network::Response::ConnectionTimeout) {}
    };

}

#endif //ROBOT_CONNECTIONTIMEOUT_HPP
