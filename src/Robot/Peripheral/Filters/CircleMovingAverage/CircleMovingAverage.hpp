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

        for (ssize_t i = 0; i < 2 * BuffSize; ++i) {
            cosValue += _buf[i];
            sinValue += _buf[++i];
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
    double _buf[2 * BuffSize]{};
    ssize_t _index;
};


#endif
