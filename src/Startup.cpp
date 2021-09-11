#include "./Startup.hpp"


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

    BNO055_I2C sensor(BNO055_ADDRESS);

    PeripheralHandler peripheralHandler("/dev/i2c-1", "/dev/ttyAMA0", UART::S115200, "/dev/spi");
    peripheralHandler.StartAsync();

    peripheralHandler.AddI2CSensor(&sensor);

    for (size_t i = 0;; ++i) {

        BNO055::Data data = sensor.GetData();
        std::cout << (ssize_t) data.CalibrationArray[0] << " "
                  << (ssize_t) data.CalibrationArray[1] << " "
                  << (ssize_t) data.CalibrationArray[2] << " "
                  << (ssize_t) data.CalibrationArray[3] << std::endl;

/*        std::cout << "Euler: "
                  << "X = " << data.EulerAngle[BNO055::X]
                  << " Y = " << data.EulerAngle[BNO055::Y]
                  << " Z = " << data.EulerAngle[BNO055::Z] << std::endl;*/

        std::cout << "Acceleration: " << std::endl
                  << "X = " << data.LinearAcceleration[BNO055::X] << std::endl
                  << "Y = " << data.LinearAcceleration[BNO055::Y] << std::endl
                  << "Z = " << data.LinearAcceleration[BNO055::Z] << std::endl;

        usleep(1000);
    }

    return 0;
}