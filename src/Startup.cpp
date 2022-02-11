#include "./Startup.hpp"

using namespace app;

[[noreturn]]
int main() {

    Robot robot;
    robot.SetUp();
    robot.Start();
//    std::thread thread(&Robot::Start, std::ref(robot));
//    thread.join();

    ///OldStart VideoProtocol in background
    DataProtocols::RobotVideoProtocol robotVideoProtocol;
    robotVideoProtocol.StartAsync();

    auto settings = StartSettings::Get();
    auto &motorsSender = settings.GetMotorsSender();
    auto &commandsReceiver = settings.GetCommandsReceiver();

    ///OldStart CommandsProtocol in synchronous mode
    DataProtocols::CommandsProtocol commands(motorsSender, commandsReceiver, robot.sensors_, robot.robotSettings_);
    commands.Start();
}