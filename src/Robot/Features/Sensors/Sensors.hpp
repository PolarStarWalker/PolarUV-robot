#ifndef ROBOT_SENSORS_HPP
#define ROBOT_SENSORS_HPP

#include <TcpSession/TcpSession.hpp>
#include "./Peripheral/PeripheralHandler/PeripheralHandler.hpp"
#include "./Peripheral/Peripheral.hpp"
#include "./Math/SIPrefix.hpp"

namespace app {

    struct SensorsStruct {
        enum Position {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

        std::array<float, 4> Rotation{};

        std::array<float, 3> Acceleration{};

        float Depth = 0.0f;

        float Pressure = 0.0f;

        float BatteryVoltage = 0.0f;

        std::array<int8_t, 4> MotionCalibration{};

        bool Euler = true;
    };

    //TODO: сделать сервис для датчиков
    class Sensors final : public lib::network::IService {
    public:
        Sensors(ssize_t id, std::string_view i2c);

        Sensors(const Sensors &) = delete;
        Sensors(Sensors &&) = delete;
        Sensors &operator=(const Sensors &) = delete;
        Sensors &operator=(Sensors &&) = delete;

    public:
        Response Read(const std::string_view &data) final;

    private:

        PeripheralHandler peripheralHandler_;

        BNO055_I2C &bno055_;
        MS5837_I2C ms5837_;

    public:

        [[nodiscard]] inline SensorsStruct GetSensorsStruct() const{
            BNO055::Data bnoData = bno055_.GetData();

            MS5837::Data msData = ms5837_.GetData();

            SensorsStruct sensorsStruct;

            sensorsStruct.Depth = msData.Depth;
            sensorsStruct.Rotation[SensorsStruct::X] = bnoData.EulerAngle[BNO055::X];
            sensorsStruct.Rotation[SensorsStruct::Y] = bnoData.EulerAngle[BNO055::Y];
            sensorsStruct.Rotation[SensorsStruct::Z] = bnoData.EulerAngle[BNO055::Z];

            sensorsStruct.Acceleration[SensorsStruct::X] = bnoData.LinearAcceleration[BNO055::X];
            sensorsStruct.Acceleration[SensorsStruct::Y] = bnoData.LinearAcceleration[BNO055::Y];
            sensorsStruct.Acceleration[SensorsStruct::Z] = bnoData.LinearAcceleration[BNO055::Z];

            sensorsStruct.MotionCalibration[0] = bnoData.CalibrationArray[0];
            sensorsStruct.MotionCalibration[1] = bnoData.CalibrationArray[1];
            sensorsStruct.MotionCalibration[2] = bnoData.CalibrationArray[2];
            sensorsStruct.MotionCalibration[3] = bnoData.CalibrationArray[3];

            return sensorsStruct;
        }
    };
}

#endif
