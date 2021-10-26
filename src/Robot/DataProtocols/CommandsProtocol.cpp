#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "../Peripheral/Peripheral.hpp"
#include "../Math/Math.hpp"

using namespace DataProtocols;

CommandsProtocol::CommandsProtocol(const char *SPIDevice, uint32_t speed_hz, size_t peripheralTimeout_us) :
        _spi(SPIDevice, speed_hz),
        _commandsSocket(1999),
        _peripheralTimeout_us(peripheralTimeout_us) {
}


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


inline std::array<char, 2 * MotorsStructLenMessage> FormMessage(const MotorsStruct &motorsStruct) {
    std::array<char, 2 * MotorsStructLenMessage> motorsMessage{};

    std::memcpy(motorsMessage.data() + 2, &motorsStruct, MotorsStructLen);
    std::memcpy(motorsMessage.data() + MotorsStructLenMessage + 2, &motorsStruct, MotorsStructLen);
    motorsMessage[0] = 's';
    motorsMessage[1] = 's';

    motorsMessage[MotorsStructLenMessage - 2] = 's';
    motorsMessage[MotorsStructLenMessage - 1] = 's';
    motorsMessage[MotorsStructLenMessage] = 's';
    motorsMessage[MotorsStructLenMessage + 1] = 's';

    motorsMessage[MotorsStructLenMessage * 2 - 2] = 's';
    motorsMessage[MotorsStructLenMessage * 2 - 1] = 's';

    return motorsMessage;
}

inline MotorsStruct FormMotorsStruct(const Vector<float> &thruster,
                                     const Vector<float> &hand,
                                     const Vector<float> &camera) {

    MotorsStruct motors;

    std::array<uint16_t, 12> hiSpeedPwm{};
    hiSpeedPwm.fill(1000);

    thruster.FillArray(&hiSpeedPwm);
    hand.FillArray(&hiSpeedPwm, thruster.Size());

    camera.FillArray(&hiSpeedPwm, thruster.Size() + hand.Size());

    std::memcpy(motors.HiSpeedPWM, hiSpeedPwm.begin(), hiSpeedPwm.size() * sizeof(uint16_t));

    return motors;
}


void CommandsProtocol::Start() {

    BNO055_I2C bno055(BNO055_ADDRESS);
    MS5837_I2C ms5837(MS5837_ADDRESS);

    PeripheralHandler peripheralHandler("/dev/i2c-1", _peripheralTimeout_us);

    peripheralHandler.AddI2CSensor(&bno055);
    peripheralHandler.AddI2CSensor(&ms5837);

    peripheralHandler.StartAsync();

    Vector<float> moveVector(6);

    for (;;) {

        _commandsSocket.Listen();

        RobotSettingsStruct settingsStruct = RobotSettingsProtocol::GetSettings();

        Matrix<float> coefficientMatrix(settingsStruct.ThrusterNumber(), 6);
        coefficientMatrix = settingsStruct.ThrusterCoefficientArray();
        coefficientMatrix *= 10;

        Vector<float> handVector(settingsStruct.HandFreedom());
        handVector = settingsStruct.HandCoefficientArray();
        handVector *= 10;

        while (_commandsSocket.IsOnline()) {

            CommandsStruct commandsStruct;
            TelemetryStruct telemetry = FormTelemetryStruct(bno055, ms5837);

            if (_commandsSocket.RecvDataLen((char *) &commandsStruct, CommandsStructLen) != CommandsStructLen)
                break;

            _commandsSocket.SendDataLen((char *) &telemetry, TelemetryStructLen);

            moveVector = commandsStruct.MoveVector;

            Vector<float> motorsCommands = coefficientMatrix * moveVector;
            motorsCommands.Normalize(1000);
            motorsCommands += 1000;

            Vector<float> handCommands(settingsStruct.HandFreedom());
            for (size_t i = 0; i < settingsStruct.HandFreedom(); i++) {
                handCommands[i] = handVector[i] * commandsStruct.TheHand[i] + 1000;
            }

            Vector<float> camera(2);
            camera = commandsStruct.Camera;
            camera *= 1000;
            camera += 1000;

            MotorsStruct motorsStruct = FormMotorsStruct(motorsCommands, handCommands, camera);

            std::array<char, 2 * MotorsStructLenMessage> motorsMessage = FormMessage(motorsStruct);
            this->_spi.ReadWrite(motorsMessage.data(), nullptr, MotorsStructLenMessage * 2);


#ifdef DEBUG

            //std::cout << telemetry << std::endl;
            //std::cout << settingsStruct << std::endl;
            //std::cout << commandsStruct << std::endl;
            //std::cout << motorsStruct << std::endl;


/*                for (size_t j = 0; j < MotorsStructLenMessage * 2; j++) {
                std::cout << motorsMessage[j] << '|';
            }
            std::cout << std::endl;
*/
#endif


        }
    }

}
