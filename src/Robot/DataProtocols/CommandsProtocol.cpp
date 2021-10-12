#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "../Peripheral/Peripheral.hpp"

using namespace DataProtocols;

CommandsProtocol::CommandsProtocol(const char *SPIDevice) : _spi(SPIDevice, 26000000), _commandsSocket(1999) {
}


inline std::array<char, 2 * MotorsStructLenMessage> FormMessage(const MotorsStruct &motorsStruct) {
    std::array<char, 2 * MotorsStructLenMessage> motorsMessage{};

    std::memcpy(motorsMessage.data() + 1, &motorsStruct, MotorsStructLen);
    std::memcpy(motorsMessage.data() + 1 + MotorsStructLenMessage, &motorsStruct, MotorsStructLen);
    motorsMessage[0] = 's';
    motorsMessage[MotorsStructLen + 1] = 's';
    motorsMessage[MotorsStructLenMessage] = 's';
    motorsMessage[MotorsStructLenMessage * 2 - 1] = 's';

    return motorsMessage;
}


inline TelemetryStruct GetTelemetryStruct(const BNO055_I2C &bno055, const MS5837_I2C &ms5837) {
    BNO055::Data bnoData = bno055.GetData();

    //MS5837::Data msData = ms5837.GetData();

    TelemetryStruct telemetryStruct;

    //telemetryStruct.Depth = msData.Depth;
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

void CommandsProtocol::Start() {


    BNO055_I2C bno055(BNO055_ADDRESS);
    MS5837_I2C ms5837(MS5837_ADDRESS);

    PeripheralHandler peripheralHandler("/dev/i2c-1");

    peripheralHandler.AddI2CSensor(&bno055);
    //peripheralHandler.AddI2CSensor(&ms5837);

    peripheralHandler.StartAsync();

    for (;;) {

        _commandsSocket.Listen();

        RobotSettingsStruct settingsStruct = RobotSettingsProtocol::GetSettings();

        FloatMatrixClass coefficientMatrix(settingsStruct.ThrusterNumber(), 6);
        coefficientMatrix = settingsStruct.ThrusterCoefficientArray();
        coefficientMatrix *= 10;

        FloatVectorClass moveVector(6);
        FloatVectorClass handVector(settingsStruct.HandFreedom());

        while (_commandsSocket.IsOnline()) {

            CommandsStruct commandsStruct;

            if (_commandsSocket.RecvDataLen((char *) &commandsStruct, CommandsStructLen) != 0) {

                TelemetryStruct telemetryStruct = GetTelemetryStruct(bno055, ms5837);
                _commandsSocket.SendDataLen((char *) &telemetryStruct, TelemetryStructLen);

                moveVector = commandsStruct.VectorArray;

                FloatVectorClass motorsCommands = coefficientMatrix * moveVector;
                motorsCommands.Normalize(1000);
                motorsCommands += 1000;

                std::array<uint16_t, 12> moveArray{};
                motorsCommands.FillArray(&moveArray);
                handVector.FillArray(&moveArray, motorsCommands.Length());

                MotorsStruct motorsStruct;
                std::memcpy(motorsStruct.PacketArray, moveArray.begin(), moveArray.size() * 2);

                std::array<char, 2 * MotorsStructLenMessage> motorsMessage = FormMessage(motorsStruct);

                this->_spi.ReadWrite(motorsMessage.data(), nullptr, MotorsStructLenMessage * 2);

#ifdef DEBUG

                //std::cout<<telemetryStruct<<std::endl;
                std::cout << settingsStruct << std::endl;
                std::cout << commandsStruct << std::endl;
                std::cout << motorsStruct << std::endl;


                /*                for (size_t i = 0; i < 2; i++) {

                                    for (size_t j = 0; j < MotorsStructLen + 1; j++) {
                                        std::cout << motorsMessage[j] << '|';
                                    }
                                    std::cout << motorsMessage[MotorsStructLen + 1] << std::endl;
                                }*/
#endif


            }
        }
    }
}
