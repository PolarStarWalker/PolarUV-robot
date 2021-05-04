#include "UART.hpp"

///ToDo: я кончено понимаю что удобнее использовать передавать 1 символ а не всю строку,
/// но UART может быть работать через юсиби, и тогда придётся писать что-то типа /dev/ttyUSB0,
/// так что на мой взгляд лучше всю строку передавать (ну либо выкрутись и сделай что-то своё:)
/// По поводу speed, в целом можно попробовать сделать по взрослому - через enum.
/// создать SpeedEnum, унаследовать его от speed_t и переписать туда тот #define со скоростями.
/// Тогда будет типизация и защита от дурака (компилятор не даст скомпилировать, если значения скорости нет в ENUM'e)
UART::UART(char portNumber, speed_t speed){
    /// Подставляем номер порта вместо Х
    this->device[9] = portNumber;

    /// Открываем порт с какими-то флагами
    ///ToDo: O_NDELAY это флаг который открывает устройство в неблокирующем режиме
    /// (как функция считывание UART_DMA по прерыванию)
    /// В целом фитча полезная, но не в нашем случае, поэтому я думаю что его стоит убрать
    this->fileDescriptor = open(this->device,O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fileDescriptor == -1){
        std::cout << "Failed to open port\n";
    }

    /// Получаем текущие настройки последовательного интерфейса
    if(tcgetattr(this->fileDescriptor, &this->attributes) < 0) {
        std::cout << "Failed to get attributes\n";
    }

    ///ToDo: во самое сложное это флаги, с ними можно долго провозиться, чтобы всё заработало
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

//ToDo: recv функция это круто, но как на счёт send?
ssize_t UART::recv(char *buff, size_t length){
    /// Считываем данные
///ToDo: вот тут я чёт не понимаю, read принимает дескриптор, указатель и длинну.
/// Buf это уже указатель, зачем брать ссылку на указатель?
    read(this->fileDescriptor, &buff, length);
}

UART::~UART(){
    /// Закрываем порт
    close(this->fileDescriptor);
}