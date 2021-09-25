#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "./TelemetryProtocol/TelemetryProtocol.hpp"

#include "../Peripheral/Peripheral.hpp"

using namespace DataProtocols;

CommandsProtocol::CommandsProtocol(const char *SPIDevice) : _spi(SPIDevice, 26000000) {
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

void CommandsProtocol::Start() {
    _commandsSocket.MakeServerSocket(1999);

    BNO055_I2C bno055(BNO055_ADDRESS);
    MS5837_I2C ms5837(MS5837_ADDRESS);

    PeripheralHandler peripheralHandler("/dev/i2c-1/","/dev/ttyAMA0", UART::S115200, "/dev/spi");

    peripheralHandler.AddI2CSensor(&bno055);
    peripheralHandler.AddI2CSensor(&ms5837);

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
                std::cout << "Struct recieved:" << std::endl;

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
                std::cout << settingsStruct;
                std::cout << motorsStruct;
                std::cout << commandsStruct;

                for (size_t i = 0; i < 2; i++) {

                    for (size_t j = 0; j < MotorsStructLen + 1; j++) {
                        std::cout << motorsMessage[j] << '|';
                    }
                    std::cout << motorsMessage[MotorsStructLen + 1] << std::endl;
                }
#endif

                BNO055::Data bnoData = bno055.GetData();
                MS5837::Data msData = ms5837.GetData();

                TelemetryStruct telemetryStruct;

                telemetryStruct.Depth = msData.Depth;
                telemetryStruct.AngleX = bnoData.EulerAngle[BNO055::X];
                telemetryStruct.AngleY = bnoData.EulerAngle[BNO055::Y];
                telemetryStruct.AngleZ = bnoData.EulerAngle[BNO055::Z];

                _commandsSocket.SendDataLen((char *) &telemetryStruct, TelemetryStructLen);
            }
        }
    }
}
