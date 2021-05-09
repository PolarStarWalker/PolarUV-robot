﻿#include "main.hpp"

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
    SettingsStruct settingsStruct = settingsFileService.GetSettings();
    ///Set socket
    Socket socket;
    socket.MakeServerSocket(1999);
    ///set spi
    SPI commandSender("/dev/spidev0.0", 26000000);
    UART uart("/dev/ttyS3", UART::S115200);

    /// program objects
    FloatVectorClass moveVector(6);

    FloatMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);

    coefficientMatrix = settingsStruct.MoveCoefficientArray;
    coefficientMatrix *= -settingsStruct.MaxCommandValue;
#ifndef NDEBUG
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
#endif

/*   /// main program
    while (settingsStruct.IsTurnOn) {
        while (socket.GetSocketConnectionStatus()) {

            ssize_t error = socket.RecvDataLen((char *) &commandsStruct, CommandsStructLen);

            if (error++) break;


        }

        for (;;) {
            std::cout << "\nВведите через пробел вектора: ";
            for (size_t i = 0; i < 6; i++) {
                std::cin >> commandsStruct.VectorArray[i];
            }

            moveVector = commandsStruct.VectorArray;
            FloatVectorClass motorsCommands = coefficientMatrix * moveVector;

            motorsCommands.Normalize(500);

            motorsCommands = motorsCommands + (1500);

            std::array<int16_t, 12> moveArray{};

            motorsCommands.FillArray(&moveArray);

            std::memcpy(motorsStruct.PacketArray, moveArray.begin(), moveArray.size() * 2);

            for (size_t i = 0; i < MotorsStructArrayLength / 2; i++) {
                std::cout << motorsStruct.PacketArray[i] << std::endl;
            }

            std::cout << MotorsStructLen << std::endl;


            std::memcpy(motorsMessage + 1, &motorsStruct, MotorsStructLen);
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

            commandSender.ReadWrite(motorsMessage, nullptr, MotorsStructLenMessage * 2);
        }

        //socket.Listen();
    }*/

    uint8_t buf[30]{};
    uart.recv(buf, 12);
    std::cout << buf << std::endl;

    return 0;
}