#ifndef ROBOT_STABILIZATION_HPP
#define ROBOT_STABILIZATION_HPP

#include "PID.hpp"
#include "CommandsStruct.hpp"
#include "../RobotSettings/RobotSettings.hpp"
#include <cmath>
#include <ranges>
#include <algorithm>
#include <iostream>

#define rad(x) ((x) * M_PI / 180)

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

    PIDArray<Size> pids_{};

public:

    stc::VerticalVector<float, 6>
    Calculate(float dt, CommandsStruct &commands, app::RobotSettingsStruct settings, app::SensorsStruct &sensors) {

        ArrayType<Size> measuredValues{sensors.Rotation[app::SensorsStruct::X],
                                       sensors.Rotation[app::SensorsStruct::Y],
                                       sensors.Rotation[app::SensorsStruct::Z],
                                       sensors.Depth};

        /// ToDo: Унифицировать диапазоны углов
        /// Перевод значений из диапазона [270; 90] в диапазон [-90; 90]
        if (measuredValues[1] >= 270.0f) measuredValues[1] -= 360.0f;

        /// Расчет значений ошибок
        auto errors = PIDArray<Size>::GetErrors(commands.StabilizationTarget, measuredValues);

        /// Проверка значений ошибок
        if (errors[0] > 180.0f) errors[0] = 360.0f - errors[0];
        else if (errors[0] < -180.0f) errors[0] = 360.0f + errors[0];
        if (errors[2] > 180.0f) errors[2] = 360.0f - errors[2];
        else if (errors[2] < -180.0f) errors[2] = 360.0f + errors[2];

        /// Расчет значений ПИД-регуляторов
        auto pids = pids_.Calculate(dt, errors);

        /// Расчет новых значений векторов движения
        float Fx = 0.0f, Fy = 0.0f, Fz = 0.0f;
        if (settings.PIDEnabled[3]) {
            Fx = (float) (-pids[3] * std::sin(rad(measuredValues[1])));
            Fy = (float) (pids[3] * std::sin(rad(measuredValues[0])) * std::cos(rad(measuredValues[0])));
            Fz = (float) (pids[3] * std::cos(rad(measuredValues[0])) * std::cos(rad(measuredValues[1])));
        }
        float Mx = (settings.PIDEnabled[0]) ? -pids[0] : 0.0f;
        float My = (settings.PIDEnabled[1]) ? -pids[1] : 0.0f;
        float Mz = (settings.PIDEnabled[2]) ? -pids[2] : 0.0f;

        stc::Vector<stc::Vertical, float, 6> newMove{Fx, Fy, Fz, Mx, My, Mz};

        /// Нормализация новых значений векторов движения
        newMove.Normalize(1.0f);

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
    }

};

#endif