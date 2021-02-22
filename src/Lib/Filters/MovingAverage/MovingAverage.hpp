#ifndef ROBOT_MOVING_AVERAGE_HPP
#define ROBOT_MOVING_AVERAGE_HPP

#include <arm_neon.h>

#include "../IFilter.hpp"

template<ssize_t BufferSize>
class MovingAverage final : public IFilter {
public:
    MovingAverage() {
        _index = 0;
    }

    double Filter(double value) final {
        if (_index == BufferSize)
            _index = 0;

        _elements[_index] = value;
        ++_index;

        double maximum = 0.0f;
        auto* elementsVector = (float64x2_t*) _elements;

        for (size_t i = 0; i < BufferSize / 2; i++)
            maximum += vaddvq_f64(elementsVector[i]);

        if(BufferSize % 2 != 0)
            for(size_t i = BufferSize - 1; i < BufferSize; i++)
                maximum += _elements[i];

        return maximum / BufferSize;
    };

private:
    double _elements[BufferSize]{};
    ssize_t _index;
};

#endif