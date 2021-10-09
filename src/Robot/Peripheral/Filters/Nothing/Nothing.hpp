#ifndef ROBOT_NOTHING_HPP
#define ROBOT_NOTHING_HPP
#include "../IFilter.hpp"

class Nothing : public IFilter{
public:
    double Filter(double value) final{
        return value;
    }
};


#endif
