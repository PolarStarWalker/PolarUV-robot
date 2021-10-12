#ifndef ROBOT_NOTHING_HPP
#define ROBOT_NOTHING_HPP
#include "../IFilter.hpp"

class Nothing final : public IFilter{
public:

    inline double Filter(double value) final{
        return value;
    }

    inline ~Nothing() final = default;
};


#endif
