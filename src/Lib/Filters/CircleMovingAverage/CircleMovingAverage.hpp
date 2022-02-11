#ifndef ROBOT_CIRCLE_MOVING_AVERAGE_HPP
#define ROBOT_CIRCLE_MOVING_AVERAGE_HPP

#include <numbers>
#include <cmath>
#include <arm_neon.h>

template<ssize_t BuffSize>
class CircleMovingAverage final : public IFilter {

    static constexpr double PI = std::numbers::pi_v<double>;

public:
    double Filter(double value) final {
        if (_index == 2 * BuffSize)
            _index = 0;

        _elements[_index] = std::cos(value);
        _elements[++_index] = std::sin(value);
        ++_index;

        auto *elementsVector = (float64x2_t *) _elements;

        double values[2] = {};
        auto &cosSin = (float64x2_t &) values;

        for (ssize_t i = 0; i < BuffSize; ++i) {
            cosSin = vaddq_f64(elementsVector[i], cosSin);
        }

        cosSin = vdivq_f64(cosSin, (float64x2_t &) div);

        ///First half
        if (values[1] > 0)
            return std::acos(values[0]);

        ///Second half
        if (values[1] < 0)
            return 2 * PI - std::acos(values[0]);

        return 0;
    }

private:
    double _elements[2 * BuffSize]{};
    ssize_t _index = 0;

    constexpr static double div[2] = {BuffSize, BuffSize};
};

#endif
