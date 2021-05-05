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
    CommandsStruct *commandsStruct = new CommandsStruct;
    MotorsStruct *motorsStruct = new MotorsStruct;

    ///bufer
    char *motorsMessage = new char[2 * MotorsStructLenMessage];
    ///settings from settings file
    SettingsFileService settingsFileService("settings");
    SettingsStruct settingsStruct = settingsFileService.GetSettings();
    ///Set socket
    Socket socket;
    socket.MakeServerSocket(1999);
    ///set spi
    SPI commandSender("/dev/spidev0.0", 35000000);

    /// program objects
    FloatVectorClass moveVector(6);
    FloatMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);
    FloatVectorClass motorsCommands;

    coefficientMatrix = settingsStruct.MoveCoefficientArray;
    coefficientMatrix = coefficientMatrix * -settingsStruct.MaxCommandValue;
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

    /// main program
    while (settingsStruct.IsTurnOn) {
        while (socket.GetSocketConnectionStatus()) {

            ssize_t error = socket.RecvDataLen((char *) commandsStruct, CommandsStructLen);

            if (error++) break;


        }

        ///ToDo: весь этот цикл ты можешь портить как хочешь,
        /// всё равно в отдельной ветке работаешь
        for (;;) {
            std::cout << "\nВведите через пробел вектора: ";
            for (size_t i = 0; i < 6; i++) {
                std::cin >> commandsStruct->VectorArray[i];
            }

            moveVector = commandsStruct->VectorArray;
            motorsCommands = coefficientMatrix * moveVector;

            motorsCommands.Normalize(500);

            motorsCommands = motorsCommands + (1500);

            std::array<int16_t, 12> moveArray{};

            motorsCommands.FillArray(&moveArray);

            std::memcpy(motorsStruct->PacketArray, moveArray.begin(), moveArray.size() * 2);

            for (size_t i = 0; i < MotorsStructArrayLength / 2; i++) {
                std::cout << motorsStruct->PacketArray[i] << std::endl;
            }

            std::cout << MotorsStructLen << std::endl;


            std::memcpy(motorsMessage + 1, motorsStruct, MotorsStructLen);
            std::memcpy(motorsMessage + 1 + MotorsStructLenMessage , motorsStruct, MotorsStructLen);
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
    }

    delete commandsStruct;
    delete motorsStruct;
    delete[] motorsMessage;

    /// ---------- Проверка ----------
    char buff[12] = "";
    UART myUART = UART('4',9600);
    while(1) {
        myUART.recv(buff, 12);
        std::cout << buff << std::endl;
    }
    /// ------------------------------

    return 0;
}


///ToDo: Описание задания
///Нужно написать класс который инкапсулирует в себе работу с UART
///по итогу должно получится что-то наподобие SPI, когда:
///1. Создаём объект через конструктор
///2. Уничтожаем его в деструкторе
///3. Имеем простые методы recv(void* ptr, size_t length) и send(const void* ptr, size_t length) для приёма и отправки данных
///Заголовочник для работы с UART называется termios.h, потом понакидаю ссылок, когда появятся ещё ToDo'шки, это значит что я подготовил проект
///Проверять можно через ардуину и её аппаратный UART, ссылку на вики сделаю также потом