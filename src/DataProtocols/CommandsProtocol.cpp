#include "./CommandsProtocol/CommandsProtocol.hpp"
#include "../Matrix/include/FloatVectorClass.hpp"

CommandsProtocol::CommandsProtocol(char *SPIDevice) : _spi(SPIDevice, 26000000){
}

void CommandsProtocol::Start() {
    ///settings from settings file
    SettingsFileService settingsFileService("settings");
    OldSettingsStruct oldSettingsStruct = settingsFileService.GetSettings();

    FloatVectorClass moveVector(6);
    FloatMatrixClass coefficientMatrix(oldSettingsStruct.ThrustersNumber, 6);
    coefficientMatrix = oldSettingsStruct.MoveCoefficientArray;
    coefficientMatrix *= 10;

    OldSettingsStruct settingsStruct{};
    settingsStruct.HandFreedom = oldSettingsStruct.HandFreedom;
    settingsStruct.ThrustersNumber = oldSettingsStruct.ThrustersNumber;
    settingsStruct.MoveCoefficientArray = oldSettingsStruct.MoveCoefficientArray;
    settingsStruct.HandCoefficientArray = oldSettingsStruct.HandCoefficientArray;
    settingsStruct.MaxMotorSpeed = oldSettingsStruct.MaxMotorSpeed;

    _socket.Listen();

    while (oldSettingsStruct.IsTurnOn) {
        while (_socket.IsOnline()) {

            CommandsStruct commandsStruct;

            if (_socket.RecvDataLen((char *) &commandsStruct, CommandsStructLen) != 0) {
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

                commandSender.ReadWrite(motorsMessage, nullptr, MotorsStructLenMessage * 2);*/
            }
        }

        _socket.Listen();
    }
}
