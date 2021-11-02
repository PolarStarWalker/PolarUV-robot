#include "./Startup.hpp"
#include "./Robot/Math/Math.hpp"

[[noreturn]]
int main() {
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

    ///Create motors sender
    static const class MotorsSender::SPI motorsSender("/dev/spidev0.0", Mega(35));

    ///Create commandReceiver commands commandReceiver
    static const class CommandsReceiver::Network commandReceiver(1999);

    ///Create peripheralHandler
    static const PeripheralHandler peripheralHandler("/dev/i2c-1", Kilo(20));

    ///OldStart CommandsProtocol in synchronous mode
    DataProtocols::CommandsProtocol commands(motorsSender, commandReceiver, peripheralHandler);
    commands.Start();
}