#include "./Startup.hpp"
#include "./Robot/Math/Math.hpp"

[[noreturn]]
int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    ///Start SettingsProtocol in background
    DataProtocols::RobotSettingsProtocol settingsProtocol;
    settingsProtocol.StartAsync();

    ///Start VideoProtocol in background
    DataProtocols::RobotVideoProtocol robotVideoProtocol;
    robotVideoProtocol.StartAsync();

    ///Start CommandsProtocol in synchronous mode
    DataProtocols::CommandsProtocol commands("/dev/spidev0.0", Mega(35), 0);
    commands.Start();

}