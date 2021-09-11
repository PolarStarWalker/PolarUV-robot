#include "Startup.hpp"

int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    Robot robot();

    ///Start SettingsProtocol in background
    DataProtocols::RobotSettingsProtocol settingsProtocol;
    settingsProtocol.StartAsync();

    ///Start VideoProtocol in background
    DataProtocols::RobotVideoProtocol robotVideoProtocol;
    robotVideoProtocol.StartAsync();

    ///Start CommandsProtocol in synchronous mode
//    DataProtocols::CommandsProtocol commands("/dev/spidev0.0");
//    commands.Start();

    BNO055_I2C bno055_i2c(0x29);

    PeripheralHandler sensorsHandler("/dev/i2c-1",
                                  "/dev/ttyAMA0",
                                  UART::S115200,
                                  "/dev/spidev0.1",
                                  25000000);

    sensorsHandler.AddI2CSensor(&bno055_i2c);

    sensorsHandler.Start();

    for(;;){

    }


    return 0;
}