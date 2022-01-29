#include "Commands.hpp"

using namespace app;

CommandsService::CommandsService(ssize_t id, std::shared_ptr<Sensors> sensors, std::shared_ptr<RobotSettings> settings) :
        lib::network::IService(id),
        sensors_(std::move(sensors)),
        settings_(std::move(settings)){}

lib::network::Response CommandsService::WriteRead(std::string_view &data) {

    const auto& commands = *((CommandsStruct*) data.data());

    const auto& settings = settings_->GetSettings();

    auto hiPWM = settings.ThrustersCoefficientArray * commands.Move;

    for(size_t i = 0; i < settings.HandFreedom; ++i)
        hiPWM[i + settings.ThrustersNumber] = settings.HandCoefficientArray[i] * commands.Hand[i];

    std::string response(sizeof(SensorsStruct), 0);
    *((SensorsStruct*) response.c_str()) = sensors_->GetSensorsStruct();

    return {std::move(response), lib::network::Response::Ok, serviceId_};
}
