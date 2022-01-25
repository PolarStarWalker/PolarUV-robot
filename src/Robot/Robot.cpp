#include <sched.h>
#include "Robot.hpp"
#include "Lib.hpp"

using namespace app;

///set max sched priority
inline void SetProcessMaxPriority() {
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);
}

inline void InitService() {
    using IService = lib::network::IService;

    IService::RegisterService<Video>(1);

    auto& settings = IService::RegisterService<RobotSettings>(0, "robot-settings.json");
    auto& sensors = IService::RegisterService<Sensors>(2, "/dev/i2c-1");

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
