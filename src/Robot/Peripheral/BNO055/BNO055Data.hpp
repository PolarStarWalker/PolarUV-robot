#ifndef ROBOT_BNO055DATA_HPP
#define ROBOT_BNO055DATA_HPP

#include "../Interfaces/IData.hpp"

struct BNO055Data : IData {
    std::array<uint8_t,4> calibrationArray{};

    std::array<double,3> accelerationArray{};

    std::array<double,3> eulerArray{};

    int8_t temperature = 0;
};

#endif //ROBOT_BNO055DATA_HPP
