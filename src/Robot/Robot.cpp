#include "Robot.hpp"

using namespace app;
using IService = lib::network::IService;

Robot::Robot() :
        network_(),
        startSettings_("/etc/polar-uv/start-settings.json") {}


[[nodiscard]]
inline std::vector<std::shared_ptr<lib::network::IService>> MakeROV(lib::network::TcpSession &network,
                                                                    StartSettings& startSettings) {

    std::vector<std::shared_ptr<lib::network::IService>> services;
    services.reserve(10);

    auto settings = network.CreateService<RobotSettings>(0, "/etc/polar-uv/move-settings");
    auto sensors = network.CreateService<Sensors>(1, startSettings.GetSensorsPath());

    services.push_back(settings);
    services.push_back(sensors);
    services.push_back(network.CreateService<Video>(2));
    services.push_back(network.CreateService<CommandsService>(3, startSettings.GetMotorsSender(), sensors, settings));

    return services;
}

void Robot::SetUp() {

    switch (startSettings_.GetMode()) {

        case StartSettings::AUV: {
            throw lib::exceptions::BaseException("[AUV NOT SUPPORTED]", lib::network::Response::NotFound);
        }
        case StartSettings::ROV: {
            services_ = MakeROV(network_, startSettings_);
            break;
        }
        default:
            throw lib::exceptions::BaseException("[MODE NOT FOUND]", lib::network::Response::NotFound);
    }
}


void Robot::Start() {
    network_.Start();
}




