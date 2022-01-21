#ifndef ROBOT_FILTERSGROUP_HPP
#define ROBOT_FILTERSGROUP_HPP

#include <type_traits>

#include "./IFilter.hpp"
#include "./MovingAverage/MovingAverage.hpp"
#include "./Nothing/Nothing.hpp"
#include "./CircleMovingAverage/CircleMovingAverage.hpp"


template<size_t Size>
class FiltersGroup {
public:

    FiltersGroup() = default;

    FiltersGroup(std::initializer_list<IFilter *> filters) {
        for (size_t i = 0; i < Size; ++i)
            _filters[i] = filters.begin()[i];
    }

    ~FiltersGroup() {
        for (auto &&filter: _filters) delete filter;
    }

    IFilter &operator[](size_t index) {
        return *(_filters[index]);
    }

private:
    IFilter *_filters[Size]{};
};

template<std::same_as<IFilter *> ... Filters>
FiltersGroup(Filters...) -> FiltersGroup<1 + sizeof...(Filters)>;

#endif