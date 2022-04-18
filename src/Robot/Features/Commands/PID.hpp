#ifndef ROBOT_PID_HPP
#define ROBOT_PID_HPP

class Timer {
    using TimePointType = std::chrono::steady_clock::time_point;
    TimePointType timePoint_ = std::chrono::steady_clock::now();
public:
    using DtType = double;

    inline DtType Update() {
        TimePointType newTime = std::chrono::steady_clock::now();
        auto oldTime = std::exchange(timePoint_, newTime);
        double dt_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - oldTime).count();
        return Nano(dt_ns);
    }
};

class P {
    double kp_;
public:
    P(double kp = 0.0) : kp_(kp) {}

    P& operator=(double kp){
        kp_ = kp;
        return *this;
    }

    [[nodiscard]] inline double Update(double err) const & noexcept {
        return err * kp_;
    }
};

class I {
    double ki_;
    double i_ = 0.0;
public:
    I(double ki = 0.0) : ki_(ki) {}

    I& operator=(double ki){
        ki_ = ki;
        return *this;
    }

    [[nodiscard]] inline double Update(Timer::DtType dt, double err) & noexcept {
        i_ += err * dt * ki_;
        return i_;
    }
};

class D {
    double kd_;
    double prevErr = 0.0;

public:
    D(double kd = 0.0) : kd_(kd) {}

    D& operator=(double kd){
        kd_ = kd;
        return *this;
    }

    [[nodiscard]] inline double Update(Timer::DtType dt, double err) & noexcept {
        auto d = (err - prevErr) * kd_ / dt;
        prevErr = err;
        return d;
    }
};

class PID {
    P p_ = 0;
    I i_ = 0;
    D d_ = 0;

public:

    PID(double kp, double ki, double kd) : p_(kp), i_(ki), d_(kd) {}

    inline double GetValue(Timer::DtType dt, double input, double setPoint) &{
        double err = setPoint - input;
        auto p = p_.Update(err);
        auto i = i_.Update(dt, err);
        auto d = d_.Update(dt, err);

        auto pid = p + i + d;

        return pid;
    }
};

template<size_t Size>
class PIDArray {

    std::array<P, Size> pArray_{};
    std::array<I, Size> iArray_{};
    std::array<D, Size> dArray_{};

public:

    using ArrayType = std::array<double, Size>;

    //ToDo: Векторизовать!
    [[nodiscard]] inline ArrayType GetValues(Timer::DtType dt, const ArrayType &errors) {

        ArrayType output{};

        for (size_t i = 0; i < Size; ++i)
            output[i] += pArray_[i].Update(errors[i]);

        for (size_t i = 0; i < Size; ++i)
            output[i] += iArray_[i].Update(dt, errors[i]);

        for (size_t i = 0; i < Size; ++i)
            output[i] += dArray_[i].Update(dt, errors[i]);

        return output;
    }
};

#endif
