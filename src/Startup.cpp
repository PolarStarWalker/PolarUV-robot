#include "Startup.hpp"

int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    Robot robot();

    DataProtocols::RobotSettingsProtocol settings;

    settings.Listen();


    return 0;
}