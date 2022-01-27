#include <sched.h>
#include "Robot.hpp"
#include "Lib.hpp"

using namespace app;
using IService = lib::network::IService;

Robot::Robot() :
        robotSettings_(IService::RegisterService<RobotSettings>(0, "robot-settings.json")),
        sensors_(IService::RegisterService<Sensors>(2, "/dev/i2c-1")) {

    IService::RegisterService<Video>(1);
}

///set max sched priority
inline void SetProcessMaxPriority() {
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);
}

inline void InitService() {


}


void Robot::SetUp() {
    SetProcessMaxPriority();

    lib::Initialize();

    InitService();
}


void Robot::Start() {
    auto &network = lib::network::TcpSession::GetInstance();
    network.Start();
}

