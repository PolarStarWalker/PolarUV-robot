#ifndef ROBOT_STABILIZATION_HPP
#define ROBOT_STABILIZATION_HPP

#include "PID.hpp"

class Stabilization {
public:
    using ArrayType = std::array<double, 3>;
private:

    ArrayType axis_;
    double kv_;

public:


    Stabilization() :
            axis_({}),
            kv_(0.0) {}

    ArrayType IntegrateVelocity(Timer::DtType dt, const ArrayType &velocity) {
        ArrayType newAngle{};

        for (size_t i = 0; i < 3; i++)
            newAngle[i] = velocity[i] * kv_ * dt + axis_[i];

        return newAngle;
    }

    static ArrayType GetErrors(const ArrayType &angles, const ArrayType &sensors) {
        ArrayType errors{};
        for (size_t i = 0; i < 3; ++i)
            errors[i] = angles[i] - sensors[i];

        return errors;
    }


    void SetAngles(const ArrayType &angles) {
        axis_ = angles;
    }

};

#endif