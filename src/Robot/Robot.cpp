#include <sched.h>
#include "Robot.hpp"

using namespace app;
using IService = lib::network::IService;

///set max sched priority
inline void SetProcessMaxPriority() {
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);
}

Robot::Robot() :
        network_(),
        startSettings_(),
        motorsSender_(startSettings_.GetMotorsSender()) {}


void Robot::SetUp() {
    robotSettings_ = network_.CreateService<RobotSettings>(0, "robot-settings.json");
    sensors_ = network_.CreateService<Sensors>(1, "/dev/i2c-1");
    video_ = network_.CreateService<Video>(2);
    commandsService_ = network_.CreateService<CommandsService>(3, motorsSender_, sensors_, robotSettings_);
}


void Robot::Start() {
    network_.Start();
}




