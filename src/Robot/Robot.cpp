#include <sched.h>
#include "Robot.hpp"
#include "./Features/RobotSettings/RobotSettings.hpp"
#include "Lib.hpp"

using namespace app;

///set max sched priority
inline void SetProcessMaxPriority(){
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);
}


void Robot::SetUp() {
    SetProcessMaxPriority();

    lib::Initialize();
    lib::network::IService::RegisterService<RobotSettings>(0, "robot-settings.json");
}



void Robot::Start() {
    auto& network = lib::network::TcpSession::GetInstance();
    network.Start();

}
