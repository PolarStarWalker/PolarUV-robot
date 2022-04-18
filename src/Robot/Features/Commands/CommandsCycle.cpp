#include "CommandsCycle.hpp"

using namespace app;

constexpr auto X = SensorsStruct::Position::X;
constexpr auto Y = SensorsStruct::Position::Y;
constexpr auto Z = SensorsStruct::Position::Z;

inline MotorsSender::MotorsStruct FormMotorsStruct(const std::array<float, 12> &hiPwm,
                                                   const std::array<float, 4> &lowPwm) {

    MotorsSender::MotorsStruct motors;

    for (size_t i = 0; i < 12; ++i)
        motors.HiPWM[i] = std::ceil(hiPwm[i]);

    for (size_t i = 0; i < 4; ++i)
        motors.LowPWM[i] = std::ceil(lowPwm[i]);

    return motors;
}

CommandsCycle::CommandsCycle(MotorsSender::IMotorsSender *motorsSender,
                             std::shared_ptr<Sensors> sensors,
                             std::shared_ptr<RobotSettings> settings) :
        motorsSender_(motorsSender),
        thread_(&CommandsCycle::StartCommands, this),
        sensors_(std::move(sensors)),
        settings_(std::move(settings)),
        isNotDone_(true),
        timer_(),
        pids_(),
        stabilization_() {};

CommandsCycle::~CommandsCycle() {
    isNotDone_.store(false);
    thread_.join();
}

void CommandsCycle::StartCommands() {

    while (isNotDone_.load()) {

        auto dt = timer_.Update();

        auto commands = GetCommands();

        auto sensorsStruct = sensors_->GetSensorsStruct();
        std::array<double, 3> position{sensorsStruct.Rotation[X], sensorsStruct.Rotation[Y], sensorsStruct.Rotation[Z]};

        if (commands.Stabilization != CommandsStruct::None) {

            constexpr auto Mx = CommandsStruct::MoveDimensionsEnum::Mx;
            constexpr auto My = CommandsStruct::MoveDimensionsEnum::My;
            constexpr auto Mz = CommandsStruct::MoveDimensionsEnum::Mz;

            const std::array<double, 3> inputV{commands.Move[Mx], commands.Move[My], commands.Move[Mz]};
            const auto inputAngles = stabilization_.IntegrateVelocity(dt, inputV);

            const auto errors = Stabilization::GetErrors(inputAngles, position);

            position = pids_.GetValues(dt, errors);

            ///ToDo: нужно дифференцировать?
            commands.Move[Mx] = position[0];
            commands.Move[My] = position[1];
            commands.Move[Mz] = position[2];
        }

        stabilization_.SetAngles(position);

        const auto settings = settings_->GetSettings();

        auto hiPWM = settings.ThrustersCoefficientArray * commands.Move;
        hiPWM.Normalize(100.0f);

        for (size_t i = 0; i < settings.HandFreedom; ++i)
            hiPWM.end()[-(i + 1)] = settings.HandCoefficientArray[i] * commands.Hand[i];

        hiPWM += 100.0f;
        hiPWM *= 10.0f;

        auto motorsStruct = FormMotorsStruct((std::array<float, 12>) hiPWM, commands.LowPWM);

        //std::cout << commands << std::endl;
        //std::cout << settings << std::endl;
        //std::cout << motorsStruct << std::endl;

        motorsSender_->SendMotorsStruct(motorsStruct);

        usleep(PERIOD_US);
    }
}

