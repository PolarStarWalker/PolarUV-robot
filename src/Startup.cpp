#include "./Startup.hpp"

using namespace app;

[[noreturn]]
int main() {

    Robot robot;
    robot.SetUp();
    ///robot.Start();

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