#include "CommandsCycle.hpp"
#include <algorithm>
#include <ranges>
#include "Schedular/Event.hpp"

using namespace app;

constexpr auto X = SensorsStruct::Position::X;
constexpr auto Y = SensorsStruct::Position::Y;
constexpr auto Z = SensorsStruct::Position::Z;

inline MotorsSender::MotorsStruct FormMotorsStruct(const std::array<float, 12> &hiPwm,
                                                   const std::array<float, 4> &lowPwm) {

    auto func = [](float x) -> uint16_t { return std::ceil(x); };

    MotorsSender::MotorsStruct motors;

    std::ranges::transform(hiPwm.cbegin(), hiPwm.cend(), motors.HiPWM.begin(), func);

    std::ranges::transform(lowPwm.cbegin(), lowPwm.cend(), motors.LowPWM.begin(), func);

    return motors;
}

CommandsCycle::CommandsCycle(MotorsSender::IMotorsSender &motorsSender,
                             std::shared_ptr<Sensors> sensors,
                             std::shared_ptr<RobotSettings> settings) :
        motorsSender_(motorsSender),
        sensors_(std::move(sensors)),
        settings_(std::move(settings)),
        isNotDone_(true),
        thread_(&CommandsCycle::StartCommands, this){};

CommandsCycle::~CommandsCycle() {
    isNotDone_.store(false);
    thread_.join();
}

void CommandsCycle::StartCommands() {

    TimerType timer;
    timer.SetTimer(TimerType::SleepFor_ms(1));
    EventTracker eventTracker(10);
    eventTracker.TrackEvent(timer, 0);

    while (isNotDone_.load()) {

        auto events = eventTracker.Listen<10>(20);

        auto dt = timer_.Update();

        auto commands = GetCommands();

        auto settings = settings_->GetSettings();

        auto sensorsStruct = sensors_->GetSensorsStruct();

        if (commands.Stabilization != CommandsStruct::None) {

            constexpr auto Mx = CommandsStruct::MoveDimensionsEnum::Mx;
            constexpr auto My = CommandsStruct::MoveDimensionsEnum::My;
            constexpr auto Mz = CommandsStruct::MoveDimensionsEnum::Mz;

            constexpr auto Ax = SensorsStruct::Position::X;
            constexpr auto Ay = SensorsStruct::Position::Y;
            constexpr auto Az = SensorsStruct::Position::Z;

            stabilization_.SetPCoefficients(settings.PIDCoefficients.PArray);
            stabilization_.SetICoefficients(settings.PIDCoefficients.IArray);
            stabilization_.SetDCoefficients(settings.PIDCoefficients.DArray);

            stabilization_.UpdateAngle(dt, commands.Move[Mx], commands.Move[My], commands.Move[Mz]);

            auto values = stabilization_.PID(dt,
                                             sensorsStruct.Rotation[Ax],
                                             sensorsStruct.Rotation[Ay],
                                             sensorsStruct.Rotation[Az],
                                             sensorsStruct.Depth);

        }

        auto hiPWM = settings.ThrustersCoefficientArray * commands.Move;
        hiPWM.Normalize(100.0f);

        auto handBegin = commands.Hand.cbegin();
        auto handEnd = commands.Hand.cbegin() + settings.HandFreedom;

        std::transform(handBegin, handEnd,
                       settings.HandCoefficientArray.cbegin(),
                       hiPWM.rbegin(),
                       [](float hand, float coefficient) { return hand * coefficient; });

        hiPWM += 100.0f;
        hiPWM *= 10.0f;

        auto motorsStruct = FormMotorsStruct((std::array<float, 12>) hiPWM, commands.LowPWM);

        motorsSender_.SendMotorsStruct(motorsStruct);

        usleep(PERIOD_US);
    }
}

