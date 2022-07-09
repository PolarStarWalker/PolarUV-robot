#include "CommandsCycle.hpp"
#include <algorithm>
#include <ranges>
#include "Schedular/Event.hpp"

using namespace app;

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
        thread_(&CommandsCycle::StartCommands, this) {}

CommandsCycle::~CommandsCycle() {
    isNotDone_.store(false);
    thread_.join();
}

void CommandsCycle::StartCommands() {

//    TimerType timer;
//    timer.SetTimer(TimerType::SleepFor_ms(1));
//    EventTracker eventTracker(10);
//    eventTracker.TrackEvent(timer, 0);

    while (isNotDone_.load()) {

        //auto events = eventTracker.Listen<10>(20);

        auto dt = timer_.Update();

        auto commands = GetCommands();

        auto settings = settings_->GetSettings();

        auto sensorsStruct = sensors_->GetSensorsStruct();

        if (commands.Stabilize) {

            stabilization_.SetPCoefficients(settings.PIDCoefficients.PArray);
            stabilization_.SetICoefficients(settings.PIDCoefficients.IArray);
            stabilization_.SetDCoefficients(settings.PIDCoefficients.DArray);

            ///ToDo DanSho смотри отсюда
            commands.Move = stabilization_.Calculate(dt, commands.Move, sensorsStruct);

        } else {
            stabilization_.Reset(sensorsStruct);
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

        auto motorsStruct = FormMotorsStruct((const std::array<float, 12> &) hiPWM, commands.LowPWM);

        motorsSender_.SendMotorsStruct(motorsStruct);

        usleep(PERIOD_US);
    }

}

