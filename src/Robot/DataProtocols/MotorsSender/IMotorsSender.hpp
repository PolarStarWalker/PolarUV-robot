#ifndef ROBOT_IMOTORSSENDER_HPP
#define ROBOT_IMOTORSSENDER_HPP

#include "../../DataStructs/MotorsStruct/MotorsStruct.hpp"

namespace MotorsSender {

    class IMotorsSender {
    public:
        virtual bool SendMotorsStruct(const MotorsStruct &motorsStruct) const = 0;

        virtual ~IMotorsSender() = default;
    };

}
#endif