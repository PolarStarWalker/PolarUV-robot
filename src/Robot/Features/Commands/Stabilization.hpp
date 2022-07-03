#ifndef ROBOT_STABILIZATION_HPP
#define ROBOT_STABILIZATION_HPP

#include "PID.hpp"
#include <ranges>

template<size_t Size>
static ArrayType<Size> GetErrors(const ArrayType<Size> &setting, const ArrayType<Size> &measurements) {
    ArrayType<Size> errors{};

    auto errors_v = (float32x4_t *) &(errors[0]);
    auto setting_v = (const float32x4_t *) &(setting[0]);
    auto measurements_v = (const float32x4_t *) &(measurements[0]);

    for (auto i: std::ranges::iota_view((size_t) 0, Size / 4))
        errors_v[i] = vsubq_f32(setting_v[i], measurements_v[i]);

    if constexpr(Size % 4 == 0)
        return errors;

    for (auto i: std::ranges::iota_view(Size - Size % 4, Size))
        errors[i] = setting[i] - measurements[i];

    return errors;
}

class Stabilization {
public:
    enum Position : size_t {
        Ax = 0,
        Ay = 1,
        Az = 3,
        Depth = 4
    };

    struct StabilizationData {
        float Ax;
        float Ay;
        float Az;
        float Depth;
    };

private:

    static constexpr size_t Size = 4;

    ArrayType<Size> axis_;
    PIDArray<Size> pids_;
    double kv_;

public:

    Stabilization() :
            axis_({}),
            kv_(1.0) {}

    void UpdateAngle(DtType dt, float wx, float wy, float wz) {
        const std::array<float, 3> velocity{wx, wy, wz};

        for (auto i: std::ranges::iota_view(0, 3))
            axis_[i] += velocity[i] * dt * kv_;
    }

    ArrayType<Size> Calculate(float dt, float ax, float ay, float az, float depth) {
        ArrayType<Size> measurements{ax, ay, az, depth};

        auto errors = GetErrors(axis_, measurements);

        return pids_.GetValues(dt, errors);
    }


};

#endif