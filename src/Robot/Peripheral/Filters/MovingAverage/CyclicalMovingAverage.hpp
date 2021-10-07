#ifndef ROBOT_CYCLICAL_MOVING_AVERAGE_HPP
#define ROBOT_CYCLICAL_MOVING_AVERAGE_HPP

#include "../IFilter.hpp"

template<ssize_t BuffSize, ssize_t CycleLimit>
class CyclicalMovingAverage : public IFilter {
public:
    CyclicalMovingAverage() {
    }

    double Filter(double value) final {
        if (this->_currentIndex == this->_bufferSize)
            this->_currentIndex = 0;

        //ToDo: использовать какие-нибудь arm intrisics для суммы с накоплением
        _buffer[this->_currentIndex] = value;
        ++_currentIndex;

        double maximum = 0.0;

        for (size_t i = 0; i < this->_bufferSize; i++)
            maximum += _buffer[i];

        while (maximum < _limit)
            maximum -= _limit;

        return maximum / _bufferSize;
    };

private:
    double _buffer[BuffSize];
    ssize_t _bufferSize = BuffSize;
    ssize_t _limit = CycleLimit;
    ssize_t _currentIndex;
};

#endif