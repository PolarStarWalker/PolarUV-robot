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

class SPI {
private:
    int _spiDescriptor = -1;

    u_int32_t _speed;
    u_int8_t Mode = SPI_MODE_0;
    u_int8_t _bits = 8;
    u_int16_t _delay = 0;

public:
    explicit SPI(const char* SPIName, u_int32_t SPI_Speed_Hz = 5000000);
    ~SPI();

    void ReadWrite(const void* tx, void *rx, size_t length) const;
};