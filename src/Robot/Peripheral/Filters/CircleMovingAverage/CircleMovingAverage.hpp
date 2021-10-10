#ifndef ROBOT_CIRCLE_MOVING_AVERAGE_HPP
#define ROBOT_CIRCLE_MOVING_AVERAGE_HPP

#include <cmath>

template<ssize_t BuffSize>
class CircleMovingAverage : public IFilter {
public:
    double Filter(double value) final {
        if (_index == BuffSize)
            _index = 0;

        _cos[_index] = std::cos(value);
        _sin[_index] = std::sin(value);
        _index++;

        double sinValue = 0;
        double cosValue = 0;

        for (ssize_t i = 0; i < BuffSize; i++) {
            sinValue += _sin[i];
            cosValue += _cos[i];
        }

        sinValue /= BuffSize;
        cosValue /= BuffSize;

        double sinProjection = std::asin(sinValue);
        double cosProjection = std::acos(cosValue);

        ///First quarter
        if (sinProjection >= 0 && cosProjection <= M_PI_2f64)
            return sinProjection;

        ///Second quarter
        if (sinProjection >= 0 && cosProjection >= M_PI_2f64)
            return cosProjection;

        ///Third quarter
        if (sinProjection <= 0 && cosProjection >= M_PI_2f64)
            return 2 * M_PIf64 - cosProjection;

        ///Forth quarter
        if (sinProjection <= 0 && cosProjection <= M_PI_2f64)
            return sinProjection + 2 * M_PIf64;

        throw std::exception();
    }

private:
    double _cos[BuffSize]{};
    double _sin[BuffSize]{};
    ssize_t _index;
};


#endif
