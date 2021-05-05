#include "UART.hpp"

///ToDo: я кончено понимаю что удобнее использовать передавать 1 символ а не всю строку,
/// но UART может быть работать через юсиби, и тогда придётся писать что-то типа /dev/ttyUSB0,
/// так что на мой взгляд лучше всю строку передавать (ну либо выкрутись и сделай что-то своё:)
/// По поводу speed, в целом можно попробовать сделать по взрослому - через enum.
/// создать SpeedEnum, унаследовать его от speed_t и переписать туда тот #define со скоростями.
/// Тогда будет типизация и защита от дурака (компилятор не даст скомпилировать, если значения скорости нет в ENUM'e)
UART::UART(const char* portNumber, UARTSpeedEnum speed){
    /// Подставляем номер порта вместо Х

    /// Открываем порт с какими-то флагами
    ///ToDo: O_NDELAY это флаг который открывает устройство в неблокирующем режиме
    /// (как функция считывание UART_DMA по прерыванию)
    /// В целом фитча полезная, но не в нашем случае, поэтому я думаю что его стоит убрать
    this->fileDescriptor = open(portNumber, O_RDWR | O_NOCTTY);
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
    if(cfsetispeed(&this->attributes, speed) < 0 ||
       cfsetospeed(&this->attributes, speed) < 0) {
        std::cout << "Failed to set speed\n";
    }

    /// Сохраняем изменения настроек
    if(tcsetattr(this->fileDescriptor, TCSAFLUSH, &this->attributes) < 0) {
        std::cout << "Failed to set attributes\n";
    }
};

ssize_t UART::recv(void *ptr, size_t length){
    read(this->fileDescriptor, ptr, length);
}

UART::~UART(){
    close(this->fileDescriptor);
}