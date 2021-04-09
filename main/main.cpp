#include "main.hpp"

#include <cstring>

#include <sched.h>


int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    ///Set program
    ///structs for transfer data
    CommandsStruct *commandsStruct = new CommandsStruct;
    MotorsStruct *motorsStruct = new MotorsStruct;
    ///bufer
    char *motorsMessage = new char[MotorsStructLenMessage];
    ///settings from settings file
    SettingsStruct settingsStruct;
    SettingsFileService settingsFileService("settings");
    settingsFileService.GetSettings(&settingsStruct);
    ///Set socket
    Socket socket;
    socket.MakeServerSocket(1999);
    ///set spi
    SPIService commandSender("/dev/spidev0.0");
    commandSender.Setup();

    /// test
    IntMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);
    FloatVectorClass vectorsMatrix(6);

    vectorsMatrix = commandsStruct->VectorArray;
    coefficientMatrix = settingsStruct.MoveCoefficientArray;


    std::cout << "\n----settings in program----" << std::endl;
    std::cout << "IsTurnOn: " << settingsStruct.IsTurnOn << std::endl;
    std::cout << "ThrusterNumber: " << settingsStruct.ThrustersNumber << std::endl;
    std::cout << "MoveCoefficientMatrix: \n";
    std::cout << coefficientMatrix;
    std::cout << "HandCoefficientArray: " << std::endl;
    for (size_t i = 0; i < settingsStruct.HandFreedom; i++) {
        std::cout << settingsStruct.HandCoefficientArray[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "MaxMotorSpeed: " << settingsStruct.MaxMotorSpeed << std::endl;
    std::cout << "MotorProtocol: " << settingsStruct.MotorsProtocol << std::endl;

    /// main program
    while (settingsStruct.IsTurnOn) {
        while (socket.GetSocketConnectionStatus()) {

            int error = socket.RecvDataLen((char *) commandsStruct, CommandsStructLen);

            if (error++) break;


        }

        for (;;) {
            std::cout << "\nВведите через пробел вектора: ";
            for (size_t i = 0; i < 6; i++) {
                std::cin >> commandsStruct->VectorArray[i];
            }

            vectorsMatrix = commandsStruct->VectorArray;
            FloatVectorClass motorsCommands = coefficientMatrix * vectorsMatrix;

            std::cout << motorsCommands;

            motorsStruct->PacketArray[0] = motorsCommands[0];

            std::memcpy(motorsMessage + 1, motorsStruct, MotorsStructLen);
            motorsMessage[0] = 's';
            motorsMessage[MotorsStructLenMessage - 1] = 's';

            commandSender.Write(motorsMessage, MotorsStructLenMessage);
        }


        //socket.Listen();
    }

    delete commandsStruct;
    delete motorsStruct;
    delete[] motorsMessage;

    return 0;
}