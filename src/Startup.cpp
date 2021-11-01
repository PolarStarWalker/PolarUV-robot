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
    static const MotorsSender::SPI_MotorsSender motorsSender("/dev/spidev0.0", Mega(35));

    ///Create reciever commands receiver
    static const Network receiver(1999);

    ///Create peripheralHandler
    static const PeripheralHandler peripheralHandler("/dev/i2c-1", Kilo(20));

    ///OldStart CommandsProtocol in synchronous mode
    DataProtocols::CommandsProtocol commands(motorsSender, receiver, peripheralHandler);
    commands.Start();
}