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
    DataProtocols::CommandsProtocol commands("");
    commands.Start();

    BNO055 bno = BNO055("/dev/i2c-1",0x29);

    if (!bno.begin()) {
        std::cout << "Can't connect to BNO055" << std::endl;
        return -1;
    }

    usleep(1000 * 1000);

    bno.UseExternalCrystal(true);

    while (true) {
        std::array<double,3> accelerationArray = bno.GetLinearAcceleration();
        std::cout << "Линейное ускорение: X: " << accelerationArray[0] << " ";
        std::cout << " Y "                     << accelerationArray[1] << " ";
        std::cout << " Z "                     << accelerationArray[2] << " ";

        std::array<double,3> eulerArray = bno.GetEulerAngles();
        std::cout << "Углы Эйлера: X: " << eulerArray[0] << " ";
        std::cout << " Y "              << eulerArray[1] << " ";
        std::cout << " Z "              << eulerArray[2] << " ";

        std::array<uint8_t,4> calibrationArray = bno.GetCalibration();
        std::cout << "Калибровка: Sys: " << (int32_t)calibrationArray[0] << " ";
        std::cout << " Gyro: "            << (int32_t)calibrationArray[1] << " ";
        std::cout << " Accel: "           << (int32_t)calibrationArray[2] << " ";
        std::cout << " Mag: "             << (int32_t)calibrationArray[3] << " ";

        int8_t temperature = bno.GetTemperature();
        std::cout << "Температура: " << (int32_t) temperature << std::endl;

        usleep(100 * 1000);
    }

    return 0;
}