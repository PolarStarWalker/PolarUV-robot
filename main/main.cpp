#include "main.hpp"
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

    /// test
    IntMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);
    FloatMatrixClass vectorsMatrix(6);

    vectorsMatrix = commandsStruct->VectorArray;
    coefficientMatrix = settingsStruct.MoveCoefficientArray;


    std::cout << "\n----settings in program----" << std::endl;
    std::cout << "IsTurnOn: " << settingsStruct.IsTurnOn << std::endl;
    std::cout << "ThrusterNumber: " << settingsStruct.ThrustersNumber << std::endl;
    std::cout << "MoveCoefficientArray: \n";
    for (size_t m = 0; m < coefficientMatrix.GetRows(); m++) {
        for (size_t n = 0; n < coefficientMatrix.GetColumns(); n++) {
            std::cout << coefficientMatrix[m][n] << " ";
        }
        std::cout << "\n";
    }
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
            std::cout << "Введите через пробел вектора: ";
            for (size_t i = 0; i < 6; i++) {
                std::cin >> commandsStruct->VectorArray[i];
            }

            vectorsMatrix = commandsStruct->VectorArray;
            IntMatrixClass motorsCommands = coefficientMatrix * vectorsMatrix;

            for (size_t i = 0; i < motorsCommands.GetRows(); i++) {
                for (size_t j = 0; j < motorsCommands.GetColumns(); j++) {
                    std::cout << motorsCommands[i][j]<< ' ';
                }
                std::cout << ' ';
            }
            std::cout<<std::endl;
        }


        //socket.Listen();
    }

    delete commandsStruct;
    delete motorsStruct;
    delete[] motorsMessage;

    return 0;
}