#include "./UART/UART.hpp"

///ToDo: const и автоформатирование кода (ctrl + alt + enter)
UART::UART(const char *address, SpeedEnum speed) {
    /// Открываем порт для чтения и записи
    this->_uartDescriptor = open(address, O_RDWR | O_NOCTTY);

    /// Получаем текущие настройки последовательного интерфейса
    if (tcgetattr(this->_uartDescriptor, &this->attributes) < 0) {
#ifndef NDEBUG
        std::cout << "Failed to get attributes\n";
#endif
    }

    /// Место для установки флагов

    /// Устанавливаем скорость приема и передачи
    if (cfsetispeed(&this->attributes, speed) < 0
        ||
        cfsetospeed(&this->attributes, speed) < 0) {
#ifndef NDEBUG
        std::cout << "Failed to set speed\n";
#endif
    }

    /// Сохраняем изменения настроек
    if (tcsetattr(this->_uartDescriptor, TCSAFLUSH, &this->attributes) < 0) {
#ifndef NDEBUG
        std::cout << "Failed to set attributes\n";
#endif
    }
}

ssize_t UART::recv(void *ptr, size_t length) const {
    return read(this->_uartDescriptor, ptr, length);
}

ssize_t UART::send(const void *ptr, size_t length) const {
    return write(this->_uartDescriptor, ptr, length);
}

UART::~UART() {
    close(this->_uartDescriptor);
}