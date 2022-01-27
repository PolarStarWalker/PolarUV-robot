#ifndef ROBOT_SENSORS_HPP
#define ROBOT_SENSORS_HPP

#include "./DataTransmissions/TcpSession/TcpSession.hpp"
#include "./Peripheral/PeripheralHandler/PeripheralHandler.hpp"
#include "./Peripheral/Peripheral.hpp"

namespace app {

    struct SensorsStruct {
        enum Position {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

        float Rotation[4]{};

        float Acceleration[3]{};

        float Depth = 0.0f;

        float Pressure = 0.0f;

        float BatteryVoltage = 0.0f;

        int8_t MotionCalibration[4]{};

        bool Euler = true;
    };

    //TODO: сделать сервис для датчиков
    class Sensors final : public lib::network::IService {
    public:
        Sensors(ssize_t id, std::string_view i2c) :
                lib::network::IService(id),
                peripheralHandler_(i2c, Kilo(10)),
                bno055_(BNO055_I2C::GetInstance()),
                ms5837_(MS5837_ADDRESS) {

            peripheralHandler_.AddI2CSensor(bno055_);
            peripheralHandler_.AddI2CSensor(ms5837_);
            peripheralHandler_.Start();

        };

        Sensors(const Sensors &) = delete;

        Sensors(Sensors &&) = delete;

        Sensors &operator=(const Sensors &) = delete;

        Sensors &operator=(Sensors &&) = delete;

        inline SensorsStruct GetSensorsStruct() const{
            BNO055::Data bnoData = bno055_.GetData();

            MS5837::Data msData = ms5837_.GetData();

            SensorsStruct sensorsStruct;

            sensorsStruct.Depth = msData.Depth;
            sensorsStruct.Rotation[TelemetryStruct::X] = bnoData.EulerAngle[BNO055::X];
            sensorsStruct.Rotation[TelemetryStruct::Y] = bnoData.EulerAngle[BNO055::Y];
            sensorsStruct.Rotation[TelemetryStruct::Z] = bnoData.EulerAngle[BNO055::Z];

            sensorsStruct.Acceleration[TelemetryStruct::X] = bnoData.LinearAcceleration[BNO055::X];
            sensorsStruct.Acceleration[TelemetryStruct::Y] = bnoData.LinearAcceleration[BNO055::Y];
            sensorsStruct.Acceleration[TelemetryStruct::Z] = bnoData.LinearAcceleration[BNO055::Z];

            sensorsStruct.MotionCalibration[0] = bnoData.CalibrationArray[0];
            sensorsStruct.MotionCalibration[1] = bnoData.CalibrationArray[1];
            sensorsStruct.MotionCalibration[2] = bnoData.CalibrationArray[2];
            sensorsStruct.MotionCalibration[3] = bnoData.CalibrationArray[3];

            return sensorsStruct;
        }

    private:

        PeripheralHandler peripheralHandler_;

        BNO055_I2C &bno055_;
        MS5837_I2C ms5837_;
    };
}

#endif
