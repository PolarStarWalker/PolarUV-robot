#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "./RobotSettings/RobotSettings.hpp"

#include "Peripheral/Peripheral.hpp"
#include "Math/Math.hpp"

using namespace DataProtocols;
using namespace MotorsSender;
using namespace CommandsReceiver;

CommandsProtocol::CommandsProtocol(const IMotorsSender &motorsSender,
                                   const ICommandsReceiver &commandsReceiver,
                                   std::shared_ptr<app::Sensors> sensors)
        : _motorsSender(motorsSender),
          _commandsReceiver(commandsReceiver),
          sensors_(std::move(sensors)){}

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

    for (;;) {

        _commandsReceiver.Wait();

        const auto settings = app::RobotSettings::GetSettings();

        while (_commandsReceiver.IsOnline()) {

            const auto commands = _commandsReceiver.GetCommandsStruct();

            auto telemetry = sensors_->GetSensorsStruct();

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

//            std::cout << telemetry << std::endl;
//            std::cout << settingsStruct << std::endl;
//            std::cout << commands << std::endl;
//            std::cout << motorsStruct << std::endl;


//            for (size_t j = 0; j < MotorsStructLenMessage * 2; j++) {
//                std::cout << motorsMessage[j] << '|';
//            }
//
//            std::cout << std::endl;
#endif
        }
    }
}