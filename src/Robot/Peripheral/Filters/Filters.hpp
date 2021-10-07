#ifndef ROBOT_FILTERS_HPP
#define ROBOT_FILTERS_HPP

#include "./IFilter.hpp"
#include "./MovingAverage/MovingAverage.hpp"
#include "./MovingAverage/CyclicalMovingAverage.hpp"

template<ssize_t GroupSize, ssize_t BufferSize>
class FiltersGroup {
public:
    FiltersGroup() {
        size_t i = 0;
        //ToDo: чё-нить получше придумать
        for (; i < 3; i++){
            this->_filters[i] = new CyclicalMovingAverage<BufferSize, 360>;
        }
        for (; i < GroupSize; i++){
            this->_filters[i] = new MovingAverage<BufferSize>;
        }
    }

    ~FiltersGroup() {
        for (size_t i = 0; i < GroupSize; i++)
            delete this->_filters[i];
    }

    inline IFilter *operator[](ssize_t index) {
        return (this->_filters)[index];
    }

private:
    IFilter *_filters[GroupSize];
};

#endif