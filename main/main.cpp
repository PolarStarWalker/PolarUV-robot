#include "main.hpp"
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
    char *motorsMessage[2 * MotorsStructLenMessage]{};
    ///settings from settings file
    SettingsFileService settingsFileService("settings");
    SettingsStruct settingsStruct = settingsFileService.GetSettings();

    ///Set data protocols
    Socket socket;
    socket.MakeServerSocket(1999);
    SPI commandSender("/dev/spidev0.0", 35000000);
    UART myUART("/dev/ttyS3", S115200);

    /// program objects
    FloatVectorClass moveVector(6);
    FloatMatrixClass coefficientMatrix(settingsStruct.ThrustersNumber, 6);
    coefficientMatrix = settingsStruct.MoveCoefficientArray;
    coefficientMatrix *= -settingsStruct.MaxCommandValue;

    /////// ---------- Проверка ----------
    char buff[14] = {};
    myUART.recv(buff, 14);
    std::cout << buff << std::endl;
    /////// ------------------------------

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