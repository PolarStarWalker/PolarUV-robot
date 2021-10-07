#ifndef ROBOT_FILTERS_HPP
#define ROBOT_FILTERS_HPP

#include "./IFilter.hpp"
#include "./MovingAverage/MovingAverage.hpp"

template<ssize_t GroupSize, ssize_t BufferSize>
class FiltersGroup {
public:
    FiltersGroup() {
        this->_filters = new MovingAverage<BufferSize>[GroupSize];
    }

    ~FiltersGroup() {
        delete[] this->_filters;
    }

    inline IFilter *operator[](ssize_t index) {
        return &((this->_filters)[index]);
    }

private:
    MovingAverage<BufferSize> *_filters;
    //ssize_t _groupSize = GroupSize;
};

#endif