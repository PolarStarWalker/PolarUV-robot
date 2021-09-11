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

/*    ///Start CommandsProtocol in synchronous mode
    DataProtocols::CommandsProtocol commands("");
    commands.Start();*/

    I2C i2c("/dev/i2c-1");
    MS5837_I2C sensor(MS5837_ADDRESS);
    sensor.Init(&i2c);

    for (size_t i = 0;; ++i) {
        MS5837::Data data = sensor.GetData();
        std::cout << "X = " << data.Depth << std::endl;
        usleep(16 * 1000);
    }

    return 0;
}