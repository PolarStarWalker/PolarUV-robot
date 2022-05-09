#include "CommandsCycle.hpp"
#include <algorithm>
#include <ranges>

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
        thread_(&CommandsCycle::StartCommands, this),
        isNotDone_(true) {};

CommandsCycle::~CommandsCycle() {
    isNotDone_.store(false);
    thread_.join();
}

void CommandsCycle::StartCommands() {

    while (isNotDone_.load()) {

        auto commands = GetCommands();

        if (commands.Stabilization != CommandsStruct::None) {
            //ToDo: стабилизацию в студию
            [[maybe_unused]]
            auto sensorsStruct = sensors_->GetSensorsStruct();
        }

        auto settings = settings_->GetSettings();

        auto hiPWM = settings.ThrustersCoefficientArray * commands.Move;
        hiPWM.Normalize(100.0f);

        std::transform(commands.Hand.cbegin(),
                       commands.Hand.cbegin() + settings.HandFreedom,
                       settings.HandCoefficientArray.cbegin(),
                       hiPWM.rbegin(),
                       [](float hand, float coefficient) { return hand * coefficient; });

//        for (size_t i = 0; i < settings.HandFreedom; ++i)
//            hiPWM.end()[-(i + 1)] = settings.HandCoefficientArray[i] * commands.Hand[i];

        hiPWM += 100.0f;
        hiPWM *= 10.0f;

        auto motorsStruct = FormMotorsStruct((std::array<float, 12>) hiPWM, commands.LowPWM);

        motorsSender_.SendMotorsStruct(motorsStruct);

        usleep(PERIOD_US);
    }
}

