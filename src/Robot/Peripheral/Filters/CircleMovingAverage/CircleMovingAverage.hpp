#ifndef ROBOT_CIRCLE_MOVING_AVERAGE_HPP
#define ROBOT_CIRCLE_MOVING_AVERAGE_HPP

#include <cmath>

template<ssize_t BuffSize>
class CircleMovingAverage : public IFilter {
public:
    double Filter(double value) final {
        if (_index == 2 * BuffSize)
            _index = 0;

        _buf[_index] = std::cos(value);
        _buf[++_index] = std::sin(value);
        ++_index;

        double sinValue = 0;
        double cosValue = 0;

        for (ssize_t i = 0; i < BuffSize; ++i) {
            cosValue += _buf[2 * i];
            sinValue += _buf[2 * i + 1];
        }

        sinValue /= BuffSize;
        cosValue /= BuffSize;

        ///First quarter
        if (sinValue >= 0 && cosValue <= 0)
            return std::acos(cosValue);

        ///Second quarter
        if (sinValue >= 0 && cosValue >= 0)
            return std::acos(cosValue);

        ///Third quarter
        if (sinValue <= 0 && cosValue >= 0)
            return 2 * M_PIf64 - std::acos(cosValue);

        ///Forth quarter
        if (sinValue <= 0 && cosValue <= 0)
            return std::asin(sinValue) + 2 * M_PIf64;

        throw std::exception();
    }

private:
    double _buf[2 * BuffSize]{};
    ssize_t _index;
};


#endif