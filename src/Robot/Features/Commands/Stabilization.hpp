#ifndef ROBOT_STABILIZATION_HPP
#define ROBOT_STABILIZATION_HPP

#include "PID.hpp"
#include "CommandsStruct.hpp"
#include "../RobotSettings/RobotSettings.hpp"
#include <ranges>
#include <algorithm>

class AngleStabilization {
    ArrayType<3> setting_{};
    ArrayType<3> oldAngle_{};
    const float kv_{1.0};

public:

    ArrayType<3> UpdateAngle(float dt, float Wx, float Wy, float Wz) {

        ArrayType<3> newAngle{};
        ArrayType<3> velocity{Wx, Wy, Wz};

        for (auto i: std::ranges::iota_view(0, 3)) {
            oldAngle_[i] = setting_[i];
            setting_[i] += velocity[i] * dt * kv_;
            newAngle[i] = setting_[i];
        }

        return newAngle;
    }

    void Reset(const app::SensorsStruct &sensors) {

        static_assert(app::SensorsStruct::X < app::SensorsStruct::Y
                      &&
                      app::SensorsStruct::Y < app::SensorsStruct::Z
                      &&
                      app::SensorsStruct::Z < app::SensorsStruct::W);

        const auto tmp = sensors.Rotation | std::views::take(3);

        std::ranges::copy(tmp, setting_.begin());
    }
};

//ToDo тут реализация стабилизации по глубине
class DepthStabilization {
    float depth_ = 0.0;
    bool isStopped_ = false;
public:

    float UpdateDepth(const stc::VerticalVector<float, 6> &move, app::SensorsStruct &sensors) {

        auto isStop = std::ranges::all_of((const std::array<float, 6> &) move,
                                          [](float x) {
                                              auto tmp = std::abs(std::roundf(x * 100));
                                              auto converted = static_cast<size_t>(tmp);
                                              return converted == 0;
                                          });

        auto updateDepth = false;

        if (isStop && !std::exchange(isStopped_, isStop))
            updateDepth = true;

        if (updateDepth)
            depth_ = sensors.Depth;

        return depth_;
    }

    void Reset(const app::SensorsStruct &sensors) {
        isStopped_ = false;
        depth_ = sensors.Depth;
    }
};

class Stabilization {

    constexpr static size_t Size = 4;

    AngleStabilization angleStabilization_{};
    DepthStabilization depthStabilization_{};
    PIDArray<Size> pids_{};

public:

    stc::VerticalVector<float, 6>
    Calculate(float dt, const stc::VerticalVector<float, 6> &move, app::SensorsStruct &sensors) {

        constexpr auto ax = app::SensorsStruct::X;
        constexpr auto ay = app::SensorsStruct::Y;
        constexpr auto az = app::SensorsStruct::Z;

        constexpr auto Wx = CommandsStruct::MoveDimensionsEnum::Wx;
        constexpr auto Wy = CommandsStruct::MoveDimensionsEnum::Wy;
        constexpr auto Wz = CommandsStruct::MoveDimensionsEnum::Wz;

        auto angles = angleStabilization_.UpdateAngle(dt, move[Wx], move[Wy], move[Wz]);
        //auto depth = depthStabilization_.UpdateDepth(move, sensors);

        ArrayType<Size> setting{angles[0], angles[1], angles[2], sensors.Depth};
        ArrayType<Size> measure{sensors.Rotation[ax], sensors.Rotation[ay], sensors.Rotation[az], sensors.Depth};

        auto errors = PIDArray<Size>::GetErrors(setting, measure);
        auto pids = pids_.Calculate(dt, errors);

        //ToDo DanShoo табилизация по глубине

        stc::Vector<stc::Vertical, float, 6> newMove{move[0],
                                                     move[1],
                                                     move[2],
                                                     pids[0],
                                                     pids[1],
                                                     pids[2]};

        return newMove;
    }

    void SetPCoefficients(const ArrayType<Size> &pArray) {
        pids_.SetPCoefficients(pArray);
    }

    void SetICoefficients(const ArrayType<Size> &iArray) {
        pids_.SetICoefficients(iArray);
    }

    void SetDCoefficients(const ArrayType<Size> &dArray) {
        pids_.SetDCoefficients(dArray);
    }

    void Reset(const app::SensorsStruct &sensors) {
        pids_.Reset();
        angleStabilization_.Reset(sensors);
        depthStabilization_.Reset(sensors);
    }

};

#endif