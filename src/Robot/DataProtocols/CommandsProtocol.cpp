#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "./TelemetryProtocol/TelemetryProtocol.hpp"


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


    TelemetryProtocol telemetryProtocol;

    for (;;) {

        _commandsSocket.Listen();

        RobotSettingsStruct settingsStruct = RobotSettingsProtocol::GetSettings();

        FloatMatrixClass coefficientMatrix(settingsStruct.ThrusterNumber(), 6);
        coefficientMatrix = settingsStruct.ThrusterCoefficientArray();
        coefficientMatrix *= 10;

        FloatVectorClass moveVector(6);

        std::cout << settingsStruct;

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

                MotorsStruct motorsStruct;
                std::memcpy(motorsStruct.PacketArray, moveArray.begin(), moveArray.size() * 2);

                std::array<char, 2 * MotorsStructLenMessage> motorsMessage = FormMessage(motorsStruct);

                this->_spi.ReadWrite(motorsMessage.data(), nullptr, MotorsStructLenMessage * 2);

                std::cout << motorsStruct;
                std::cout << commandsStruct;

#ifdef DEBUG

                for (size_t i = 0; i < 2; i++) {

                    for (size_t j = 0; j < MotorsStructLen + 1; j++) {
                        std::cout << motorsMessage[j] << '|';
                    }
                    std::cout << motorsMessage[MotorsStructLen + 1] << std::endl;
                }
#endif

            }
        }
    }
}
