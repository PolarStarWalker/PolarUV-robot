#ifndef ROBOT_MOVING_AVERAGE_HPP
#define ROBOT_MOVING_AVERAGE_HPP

#include "../IFilter.hpp"

template<ssize_t BufferSize>
class MovingAverage : public IFilter {
public:
    MovingAverage() {
        this->_index = 0;
    }

    double Filter(double value) final {
        if (_index == BufferSize)
            _index = 0;

        //ToDo: использовать какие-нибудь arm intrisics для суммы с накоплением
        _buffer[_index] = value;
        ++_index;

        double maximum = 0.0;

        for (size_t i = 0; i < BufferSize; i++)
            maximum += _buffer[i];

        return maximum / BufferSize;
    };

private:
    double _buffer[BufferSize];
    ssize_t _index;
};

#endif