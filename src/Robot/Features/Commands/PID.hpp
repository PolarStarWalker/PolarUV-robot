#ifndef ROBOT_PID_HPP
#define ROBOT_PID_HPP

#include <arm_neon.h>

template<size_t Size>
using ArrayType = std::array<float, Size>;
using DtType = float;

class Timer {
    using TimePointType = std::chrono::steady_clock::time_point;
    TimePointType timePoint_ = std::chrono::steady_clock::now();
public:

    inline DtType Update() {
        TimePointType newTime = std::chrono::steady_clock::now();
        auto oldTime = std::exchange(timePoint_, newTime);
        DtType dt_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - oldTime).count();
        return Nano(dt_ns);
    }
};

#if 0
class P {
    double kp_;
public:
    P(double kp = 0.0) : kp_(kp) {}

    P &operator=(double kp) {
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

    I &operator=(double ki) {
        ki_ = ki;
        return *this;
    }

    [[nodiscard]] inline double Update(DtType dt, double err) & noexcept {
        i_ += err * dt * ki_;
        return i_;
    }
};

class D {
    double kd_;
    double prevErr = 0.0;

public:
    D(double kd = 0.0) : kd_(kd) {}

    D &operator=(double kd) {
        kd_ = kd;
        return *this;
    }

    [[nodiscard]] inline double Update(DtType dt, double err) & noexcept {
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

    inline double GetValue(DtType dt, double input, double setPoint) &{
        double err = setPoint - input;
        auto p = p_.Update(err);
        auto i = i_.Update(dt, err);
        auto d = d_.Update(dt, err);

        auto pid = p + i + d;

        return pid;
    }
};
#endif

template<size_t Size>
class PArray {
    std::array<float, Size> coefficients_;
public:

    float &operator[](size_t index) {
        return coefficients_[index];
    }

    inline void Add(const std::array<float, Size> &errors, std::array<float, Size> &out) const noexcept {

        auto coefficients_v = (const float32x4_t *) &coefficients_[0];
        auto errors_v = (const float32x4_t *) &errors[0];
        auto out_v = (float32x4_t*) &out[0];

        for (auto i: std::ranges::iota_view((size_t)0, Size / 4))
            out_v[i] = vmulq_f32(coefficients_v[i], errors_v[i]);

        if constexpr(Size % 4 == 0)
            return;

        for (auto i: std::ranges::iota_view(Size - Size % 4, Size))
            out[i] = coefficients_[i] * errors[i];

    }

};

template<size_t Size>
class IArray {
    std::array<float, Size> coefficients_;
    std::array<float, Size> is_{};
public:

    float &operator[](size_t index) {
        return coefficients_[index];
    }

    inline void Add(float dt, const std::array<float, Size> &errors, std::array<float, Size> &out) noexcept {

        const std::array<float, Size> dts {dt, dt, dt, dt};
        const auto& dt_v = *(float32x4_t *) dts.begin();

        auto coefficients_v = (const float32x4_t *) &coefficients_[0];
        auto errors_v = (const float32x4_t *) &errors[0];
        auto is_v = (float32x4_t *) &is_[0];

        auto out_v = (float32x4_t *) &out[0];

        for (auto i: std::ranges::iota_view((size_t) 0, Size / 4)) {
            auto tmp = vmulq_f32(dt_v, errors_v[i]);
            tmp = vmulq_f32(coefficients_v[i], tmp);
            is_v[i] = vaddq_f32(is_v[i], tmp);
            out_v[i] = vaddq_f32(out_v[i], is_v[i]);
        }

        if constexpr(Size % 4 == 0)
            return;

        for (auto i: std::ranges::iota_view(Size - Size % 4, Size)) {
            auto tmp = dt * errors[i];
            tmp = coefficients_[i] * tmp;
            is_[i] = is_[i] + tmp;
            out_v[i] = out_v[i] + is_[i];
        }
    }

};

template<size_t Size>
class DArray {
    std::array<float, Size> coefficients_;
    std::array<float, Size> prevErrors_{};
public:

    float &operator[](size_t index) {
        return coefficients_[index];
    }

    inline void Add(float dt, const std::array<float, Size> &errors, std::array<float, Size> &out) noexcept {

        const std::array<float, Size> dts {dt, dt, dt, dt};
        const auto& dt_v = *(float32x4_t *) dts.begin();

        auto coefficients_v = (const float32x4_t *) &coefficients_[0];
        auto errors_v =(const float32x4_t *) &errors[0];
        auto prevError_v = (float32x4_t *) &prevErrors_[0];

        auto out_v = (float32x4_t *) &out[0];

        for (auto i: std::ranges::iota_view((size_t) 0, Size / 4)) {
            auto tmp = vsubq_f32(errors_v[i], prevError_v[i]);
            tmp = vdivq_f32(tmp, dt_v);
            out_v[i] = vmulq_f32(coefficients_v[i], tmp);
            prevError_v[i] = errors_v[i];
        }

        if constexpr(Size % 4 == 0)
            return;

        for (auto i: std::ranges::iota_view(Size - Size % 4, Size)) {
            auto tmp = errors[i] - prevErrors_[i];
            tmp = tmp / dt;
            out[i] =  coefficients_[i] * tmp;
            prevErrors_[i] = errors[i];
        }
    }

};

template<size_t Size>
class PIDArray {

    PArray<Size> pArray_{};
    IArray<Size> iArray_{};
    DArray<Size> dArray_{};

public:

    [[nodiscard]] inline ArrayType<Size> GetValues(DtType dt, const ArrayType<Size> &errors) {

        ArrayType<Size> output{};

        pArray_.Add(errors, output);

        iArray_.Add(dt, errors, output);

        dArray_.Add(dt, errors, output);

        return output;
    }
};

#endif
