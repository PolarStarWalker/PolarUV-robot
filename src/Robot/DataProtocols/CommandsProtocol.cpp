#include "./CommandsProtocol/CommandsProtocol.hpp"

using namespace DataProtocols;


CommandsProtocol::CommandsProtocol(const char *SPIDevice) : _spi(SPIDevice, 26000000) {
}

void CommandsProtocol::Start() {
    ///settings from settings file
    ///ToDo Заменить на настройки с земли
    RobotSettingsProtocol settingsFileService();

    _commandsSocket.MakeServerSocket(1999);

    for (;;) {

        _commandsSocket.Listen();

        RobotSettingsStruct settingsStruct{};

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

                std::cout << motorsStruct;
                std::cout << commandsStruct;


/*              std::memcpy(motorsMessage + 1, &motorsStruct, MotorsStructLen);
                std::memcpy(motorsMessage + 1 + MotorsStructLenMessage, &motorsStruct, MotorsStructLen);
                motorsMessage[0] = 's';
                motorsMessage[MotorsStructLen + 1] = 's';
                motorsMessage[MotorsStructLenMessage] = 's';
                motorsMessage[MotorsStructLenMessage * 2 - 1] = 's';

                for (size_t i = 0; i < 2; i++) {

                    for (size_t j = 0; j < MotorsStructLen + 1; j++) {
                        std::cout << motorsMessage[j] << '|';
                    }
                    std::cout << motorsMessage[MotorsStructLen + 1] << std::endl;
                }

                this->_spi.ReadWrite(motorsMessage, nullptr, MotorsStructLenMessage * 2);*/
            }
        }
    }
}
