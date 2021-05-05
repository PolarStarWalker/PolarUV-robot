#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

///ToDo: ну понеслась
///ну конструктор, деструктор, передача, считывание

class UART {
private:
    char device[11] = "/dev/ttySX";
    int fileDescriptor = -1;
    struct termios attributes;

public:
    UART(char portNumber, speed_t speed);
    ~UART();

    ssize_t recv(void *ptr, size_t length);
};

