#pragma once
//open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//close()
#include <unistd.h>

//ioctl()
#include <sys/ioctl.h>

//spi
#include <linux/spi/spidev.h>

class SPIService {
private:
    int _spiDescriptor = -1;

    u_int32_t speed = 5000000;
    u_int8_t mode = SPI_MODE_0;
    u_int8_t bits = 8;
    u_int16_t delay = 0;

public:
    explicit SPIService(const char* SPIName);
    ~SPIService();

    void ReadWrite(const void* tx, void *rx, size_t length) const;


};