#ifndef ROBOT_IFILTER_HPP
#define ROBOT_IFILTER_HPP

class IFilter {
public:
    virtual double Filter(double value) = 0;

    virtual ~IFilter() = default;

    inline double operator()(double value) { return Filter(value); }

    IFilter() = default;

    IFilter(const IFilter &filter) = delete;

    IFilter(IFilter &&filter) = delete;
};


#endif
