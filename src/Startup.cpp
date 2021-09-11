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



/*    PeripheralHandler sensorsHandler("/dev/i2c-1",
                                     "/dev/ttyAMA0",
                                     UART::S115200,
                                     "/dev/spidev0.1",
                                     25000000);

    bool flag = sensorsHandler.AddI2CSensor(&bno055_i2c);

    sensorsHandler.StartAsync();*/

    I2C i2c("/dev/i2c-1");
    BNO055_I2C bno055_i2c(0x29);
    bool flag = bno055_i2c.Init(&i2c);

    for (size_t i = 0;; ++i) {
        usleep(1000 * 1000);
        bno055_i2c.ReadData();

        std::array<uint8_t, 4> calibrationArray = bno055_i2c.GetCalibration();
        std::cout << i << " " << "Калибровка: Sys = " << (int32_t) calibrationArray[0]
                  << " Gyro = " << (int32_t) calibrationArray[1]
                  << " Accel = " << (int32_t) calibrationArray[2]
                  << " Mag = " << (int32_t) calibrationArray[3] << std::endl;

        std::array<double, 3> array = bno055_i2c.GetEulerAngle();
        std::cout << i << " " << "Наклон: X = " << array[0] << " Y = " << array[1] << " Z = " << array[2] << std::endl;

        array = bno055_i2c.GetLinearAcceleration();
        std::cout << i << " " << "Ускорение: X = " << array[0] << " Y = " << array[1] << " Z = " << array[2] << "\n"
                  << std::endl;
    }

    return 0;
}