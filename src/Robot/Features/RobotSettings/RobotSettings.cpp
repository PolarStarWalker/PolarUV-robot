#include "RobotSettings.hpp"
#include "./RobotSettingsMessage.pb.h"

using namespace app;

RobotSettings::RobotSettings(ssize_t id, std::string_view filename) :
        lib::network::IService(id),
        filename_(filename){}


void RobotSettings::Validate() {

}

lib::network::Response RobotSettings::Write(std::string_view &data) {
    RobotSettingsMessage message;
    message.ParseFromArray(data.data(), data.size());

    std::clog << "Thrusters number: " << message.thrusters_coefficient().size()/6 << std::endl;
    std::clog << "Hand freedom number: " << message.hand_coefficient().size() << std::endl;
    std::clog << "Max motors speed: " << message.maximum_motor_speed() << std::endl;
    std::clog << "Motors protocol: " << message.motors_protocol() << std::endl;

    return {std::string(), lib::network::Response::Ok, serviceId_};
}
