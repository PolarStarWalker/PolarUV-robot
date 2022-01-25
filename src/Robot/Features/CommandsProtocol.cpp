#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "./RobotSettings/RobotSettings.hpp"

#include "Peripheral/Peripheral.hpp"
#include "Math/Math.hpp"

using namespace DataProtocols;
using namespace MotorsSender;
using namespace CommandsReceiver;

CommandsProtocol::CommandsProtocol(const IMotorsSender &motorsSender, const ICommandsReceiver &commandsReceiver,
                                   const PeripheralHandler &peripheralHandler)
        : _motorsSender(motorsSender),
          _peripheralHandler(peripheralHandler),
          _commandsReceiver(commandsReceiver){}

inline TelemetryStruct FormTelemetryStruct(const BNO055_I2C &bno055, const MS5837_I2C &ms5837) {
    BNO055::Data bnoData = bno055.GetData();

    MS5837::Data msData = ms5837.GetData();

    TelemetryStruct telemetryStruct;

    telemetryStruct.Depth = msData.Depth;
    telemetryStruct.Rotation[TelemetryStruct::X] = bnoData.EulerAngle[BNO055::X];
    telemetryStruct.Rotation[TelemetryStruct::Y] = bnoData.EulerAngle[BNO055::Y];
    telemetryStruct.Rotation[TelemetryStruct::Z] = bnoData.EulerAngle[BNO055::Z];

    telemetryStruct.Acceleration[TelemetryStruct::X] = bnoData.LinearAcceleration[BNO055::X];
    telemetryStruct.Acceleration[TelemetryStruct::Y] = bnoData.LinearAcceleration[BNO055::Y];
    telemetryStruct.Acceleration[TelemetryStruct::Z] = bnoData.LinearAcceleration[BNO055::Z];

    telemetryStruct.MotionCalibration[0] = bnoData.CalibrationArray[0];
    telemetryStruct.MotionCalibration[1] = bnoData.CalibrationArray[1];
    telemetryStruct.MotionCalibration[2] = bnoData.CalibrationArray[2];
    telemetryStruct.MotionCalibration[3] = bnoData.CalibrationArray[3];

    return telemetryStruct;
}

inline MotorsStruct FormMotorsStruct(const StaticVector<float, 12> &hiPwm,
                                     const StaticVector<float, 4> &lowPwm) {

    MotorsStruct motors;

    for (size_t i = 0; i < 12; ++i)
        motors.HiPWM[i] = std::ceil(hiPwm[i]);

    for (size_t i = 0; i < 4; ++i)
        motors.LowPWM[i] = std::ceil(lowPwm[i]);

    return motors;
}

void CommandsProtocol::Start() {
    BNO055_I2C &bno055 = BNO055_I2C::GetInstance();

    MS5837_I2C ms5837(MS5837_ADDRESS);

    _peripheralHandler.AddI2CSensor(&bno055);
    //_peripheralHandler.AddI2CSensor(&ms5837);

    _peripheralHandler.StartAsync();

    for (;;) {

        _commandsReceiver.Wait();

        const auto settings = app::RobotSettings::GetSettings();

        while (_commandsReceiver.IsOnline()) {

            const auto commands = _commandsReceiver.GetCommandsStruct();

            auto telemetry = FormTelemetryStruct(bno055, ms5837);

            _commandsReceiver.SendTelemetryStruct(telemetry);

            auto hiPWM = settings.ThrustersCoefficientArray * commands.MoveVector;
            hiPWM.Normalize(100);

            for (size_t i = 0; i < settings.HandFreedom; ++i)
                hiPWM[settings.ThrustersNumber + i] = settings.HandCoefficientArray[i] * commands.TheHand[i];

            hiPWM += static_cast<float>(100);
            hiPWM *= static_cast<float>(10);

            auto lowPWM = commands.LowPWM;
            lowPWM[0] *= 1000;
            lowPWM[0] += 1500;

            MotorsStruct motorsStruct = FormMotorsStruct(hiPWM, lowPWM);

            _motorsSender.SendMotorsStruct(motorsStruct);

#ifdef DEBUG

            //std::cout << telemetry << std::endl;
            //std::cout << settingsStruct << std::endl;
            //std::cout << commands << std::endl;
            //std::cout << motorsStruct << std::endl;


            /*for (size_t j = 0; j < MotorsStructLenMessage * 2; j++) {
                std::cout << motorsMessage[j] << '|';
            }

            std::cout << std::endl;
*/
#endif
        }
    }
}