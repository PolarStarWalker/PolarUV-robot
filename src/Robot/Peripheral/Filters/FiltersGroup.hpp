#ifndef ROBOT_FILTERSGROUP_HPP
#define ROBOT_FILTERSGROUP_HPP

#include "./IFilter.hpp"
#include "./MovingAverage/MovingAverage.hpp"
#include "./Nothing/Nothing.hpp"

template<ssize_t GroupSize>
class FiltersGroup {
public:

    ~FiltersGroup() {
        for (size_t i = 0; i < GroupSize; i++)
            delete this->_filters[i];
    }

    inline IFilter *&operator[](ssize_t index) {
        return (this->_filters)[index];
    }

private:
    IFilter *_filters[GroupSize]{};
};

#endif