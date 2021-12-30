#include "./Startup.hpp"
#include "./Robot/Math/Math.hpp"

[[noreturn]]
int main() {

    //TcpSession& network = TcpSession::GetInstance();
    //network.Start();

    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    ///OldStart SettingsProtocol in background
    DataProtocols::RobotSettingsProtocol settingsProtocol;
    settingsProtocol.StartAsync();

    ///OldStart VideoProtocol in background
    DataProtocols::RobotVideoProtocol robotVideoProtocol;
    robotVideoProtocol.StartAsync();

    auto settings = StartSettings::Get();
    auto &motorsSender = settings.GetMotorsSender();
    auto &commandsReceiver = settings.GetCommandsReceiver();

    ///Create peripheralHandler
    static const PeripheralHandler peripheralHandler("/dev/i2c-1", Kilo(20));

    ///OldStart CommandsProtocol in synchronous mode
    DataProtocols::CommandsProtocol commands(motorsSender, commandsReceiver, peripheralHandler);
    commands.Start();
}