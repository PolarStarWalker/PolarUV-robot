#include "Commands.hpp"

using namespace app;

CommandsService::CommandsService(ssize_t id, MotorsSender::IMotorsSender* motorsSender, std::shared_ptr<Sensors> sensors, std::shared_ptr<RobotSettings> settings) :
        lib::network::IService(id),
        sensors_(std::move(sensors)),
        settings_(std::move(settings)),
        motorsSender_(motorsSender){}


inline MotorsSender::MotorsStruct FormMotorsStruct(const StaticVector<float, 12> &hiPwm,
                                                   const StaticVector<float, 4> &lowPwm) {

    MotorsSender::MotorsStruct motors;

    for (size_t i = 0; i < 12; ++i)
        motors.HiPWM[i] = std::ceil(hiPwm[i]);

    for (size_t i = 0; i < 4; ++i)
        motors.LowPWM[i] = std::ceil(lowPwm[i]);

    return motors;
}

lib::network::Response CommandsService::WriteRead(std::string_view &data) {

    std::string response(sizeof(SensorsStruct), 0);
    auto& sensorsStruct = *((SensorsStruct*) response.c_str());
    sensorsStruct = sensors_->GetSensorsStruct();

    const auto& commands = *((CommandsStruct*) data.data());

    const auto& settings = settings_->GetSettings();

    auto hiPWM = settings.ThrustersCoefficientArray * commands.Move;

    for(size_t i = 0; i < settings.HandFreedom; ++i)
        hiPWM[i + settings.ThrustersNumber] = settings.HandCoefficientArray[i] * commands.Hand[i];

    hiPWM += static_cast<float>(100);
    hiPWM *= static_cast<float>(10);

    auto lowPWM = commands.LowPWM;
    lowPWM[0] *= 1000;
    lowPWM[0] += 1500;

    auto motorsStruct = FormMotorsStruct(hiPWM, lowPWM);

    motorsSender_->SendMotorsStruct(motorsStruct);

    return {std::move(response), lib::network::Response::Ok, serviceId_};
}
