#include "UART.hpp"

UART::UART(char portNumber, speed_t speed){
    /// Подставляем номер порта вместо Х
    this->device[9] = portNumber;

    /// Открываем порт с какими-то флагами
    this->fileDescriptor = open(this->device,O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fileDescriptor == -1){
        std::cout << "Failed to open port\n";
    }

    /// Получаем текущие настройки последовательного интерфейса
    if(tcgetattr(this->fileDescriptor, &this->attributes) < 0) {
        std::cout << "Failed to get attributes\n";
    }

    /*
    /// Устанавливаем флаги. В них еще не разобрался
    */

    /// Устанавливаем скорость приема и передачи
    // Почему то оно не хочет принимать скорость в виде числа speed_t
    // Работает только со своими константами вроде B9600
    // (Временное решение)
    if(cfsetispeed(&this->attributes, B9600) < 0 ||
       cfsetospeed(&this->attributes, B9600) < 0) {
        std::cout << "Failed to set speed\n";
    }

    /// Сохраняем изменения настроек
    if(tcsetattr(this->fileDescriptor, TCSAFLUSH, &this->attributes) < 0) {
        std::cout << "Failed to set attributes\n";
    }
};

ssize_t UART::recv(char *buff, size_t length){
    /// Считываем данные
    read(this->fileDescriptor, &buff, length);
}

UART::~UART(){
    /// Закрываем порт
    close(this->fileDescriptor);
}