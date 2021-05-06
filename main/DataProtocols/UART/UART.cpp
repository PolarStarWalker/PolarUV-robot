#include "UART.hpp"

///ToDo: я кончено понимаю что удобнее использовать передавать 1 символ а не всю строку,
/// но UART может быть работать через юсиби, и тогда придётся писать что-то типа /dev/ttyUSB0,
/// так что на мой взгляд лучше всю строку передавать (ну либо выкрутись и сделай что-то своё:)
/// По поводу speed, в целом можно попробовать сделать по взрослому - через enum.
/// создать SpeedEnum, унаследовать его от speed_t и переписать туда тот #define со скоростями.
/// Тогда будет типизация и защита от дурака (компилятор не даст скомпилировать, если значения скорости нет в ENUM'e)
UART::UART(const char* address, UARTSpeedEnum speed){
    /// Открываем порт для чтения и записи
    this->_uartDescriptor = open(address, O_RDWR | O_NOCTTY);
    if (this->_uartDescriptor == -1){
        std::cout << "Failed to open port\n";
    }

    /// Получаем текущие настройки последовательного интерфейса
    if(tcgetattr(this->_uartDescriptor, &this->attributes) < 0) {
        std::cout << "Failed to get attributes\n";
    }

    /// Место для установки флагов

    /// Устанавливаем скорость приема и передачи
    if(cfsetispeed(&this->attributes, speed) < 0 ||
       cfsetospeed(&this->attributes, speed) < 0) {
        std::cout << "Failed to set speed\n";
    }

    /// Сохраняем изменения настроек
    if(tcsetattr(this->_uartDescriptor, TCSAFLUSH, &this->attributes) < 0) {
        std::cout << "Failed to set attributes\n";
    }
};

ssize_t UART::recv(void *ptr, size_t length){
    return read(this->_uartDescriptor, ptr, length);
}

ssize_t UART::send(void *ptr, size_t length){
    return write(this->_uartDescriptor, ptr, length);
}

UART::~UART(){
    close(this->_uartDescriptor);
}