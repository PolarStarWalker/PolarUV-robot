#include "CommandsCycle.hpp"

using namespace app;

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
                             std::shared_ptr <Sensors> sensors,
                             std::shared_ptr <RobotSettings> settings) :
        motorsSender_(motorsSender),
        thread_(&CommandsCycle::StartCommands, this),
        sensors_(std::move(sensors)),
        settings_(std::move(settings)),
        isNotDone_(true)
        {};

CommandsCycle::~CommandsCycle(){
    isNotDone_.store(false);
    thread_.join();
}

void CommandsCycle::StartCommands(){

    while(isNotDone_.load()){

        auto commands = GetCommands();

        if (commands.Stabilization != CommandsStruct::None) {
            //ToDo: стабилизацию в студию
            auto sensorsStruct = sensors_->GetSensorsStruct();
        }

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

