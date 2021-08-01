#include "main.hpp"
#include <cstring>
#include <array>
#include <sched.h>

int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    ///Set program
    ///structs for transfer data
    CommandsStruct commandsStruct;
    MotorsStruct motorsStruct;

    ///bufer
    char motorsMessage[2 * MotorsStructLenMessage]{};
    ///settings from settings file
    SettingsFileService settingsFileService("settings");
    OldSettingsStruct oldSettingsStruct = settingsFileService.GetSettings();
    ///Set socket
    Socket socket;
    socket.MakeServerSocket(1999);
    ///set spi
    SPI commandSender("/dev/spidev0.0", 26000000);
    UART uart("/dev/ttyS3", UART::S115200);

    /// program objects
    FloatVectorClass moveVector(6);
    FloatMatrixClass coefficientMatrix(oldSettingsStruct.ThrustersNumber, 6);
    coefficientMatrix = oldSettingsStruct.MoveCoefficientArray;
    coefficientMatrix *= 10;

    SettingsStruct settingsStruct{};
    settingsStruct.HandFreedom = oldSettingsStruct.HandFreedom;
    settingsStruct.ThrustersNumber = oldSettingsStruct.ThrustersNumber;
    settingsStruct.MoveCoefficientArray = oldSettingsStruct.MoveCoefficientArray;
    settingsStruct.HandCoefficientArray = oldSettingsStruct.HandCoefficientArray;
    settingsStruct.MaxMotorSpeed = oldSettingsStruct.MaxMotorSpeed;

#ifndef NDEBUG
    std::cout << settingsStruct;
#endif

    socket.Listen();
    std::cout << "Client connected\n";

    while (oldSettingsStruct.IsTurnOn) {
        while (socket.IsOnline()) {
            if (socket.RecvDataLen((char *) &commandsStruct, CommandsStructLen) != 0) {
                std::cout << "Struct recieved:" << std::endl;

                moveVector = commandsStruct.VectorArray;

                FloatVectorClass motorsCommands = coefficientMatrix * moveVector;
                motorsCommands.Normalize(1000);
                motorsCommands += 1000;

                std::array<uint16_t, 12> moveArray{};
                motorsCommands.FillArray(&moveArray);
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

        socket.Listen();
    }

    return 0;
}