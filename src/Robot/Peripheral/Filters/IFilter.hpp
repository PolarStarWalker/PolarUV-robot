#ifndef ROBOT_IFILTER_HPP
#define ROBOT_IFILTER_HPP

class IFilter {
public:
    virtual double Filter(double value) = 0;

private:
};


#endif
