#ifndef ROBOT_FILTERSGROUP_HPP
#define ROBOT_FILTERSGROUP_HPP

#include "./IFilter.hpp"
#include "./MovingAverage/MovingAverage.hpp"
#include "./Nothing/Nothing.hpp"
#include "./CircleMovingAverage/CircleMovingAverage.hpp"

#include <tuple>

template<class ... Filters>
class FiltersGroup {
public:

    ~FiltersGroup() {
        for (size_t i = 0; i < sizeof...(Filters); i++)
            delete _filters[i];
    }

    IFilter &operator[](size_t index) {
        return *(_filters[index]);
    }

private:
    IFilter *_filters[sizeof...(Filters)]{new Filters...};

};

#endif