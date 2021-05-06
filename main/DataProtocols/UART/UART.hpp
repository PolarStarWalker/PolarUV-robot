#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

enum UARTSpeedEnum : speed_t{
    S57600 = 0010001,
    S115200 = 0010002,
    S230400 = 0010003,
    S460800 = 0010004,
    S500000 = 0010005,
    S576000 = 0010006,
    S921600 = 0010007,
    S1000000 = 0010010,
    S1152000 = 0010011,
    S1500000 = 0010012,
    S2000000 = 0010013,
    S2500000 = 0010014,
    S3000000 = 0010015,
    S3500000 = 0010016,
    S4000000 = 0010017,
};

class UART {
private:
    int _uartDescriptor = -1;
    struct termios attributes;

public:
    UART(const char* address, UARTSpeedEnum speed);
    ~UART();

    ssize_t recv(void *ptr, size_t length);
    ssize_t send(void *ptr, size_t length);
};

