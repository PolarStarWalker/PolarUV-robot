#include "RobotSettings.hpp"
#include "./RobotSettingsMessage.pb.h"

using namespace Application;

RobotSettings::RobotSettings(size_t id) :
        lib::network::IService(id) {}


void RobotSettings::Validate() {

}

lib::network::Response RobotSettings::Write(std::string_view data) {
    RobotSettingsMessage message;
    message.ParseFromString(data.begin());

    std::cout << "Thrusters number: " << message.thrusters_coefficient().size() << std::endl;
    std::cout << "Hand freedom number: " << message.hand_coefficient().size() << std::endl;
    std::cout << "Max motors speed: " << message.maximum_motor_speed() <<std::endl;
    std::cout << "Motors protocol: " << message.motors_protocol() <<std::endl;

    return {std::string(), lib::network::Response::Ok};
}
