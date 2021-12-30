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

namespace DataTransmissions {

    class SPI {
    private:
        const int _spiDescriptor = -1;

        const u_int32_t _speed;
        const u_int8_t _bits = 8;
        const u_int16_t _delay = 0;

    public:
        explicit SPI(const char *address, u_int32_t speedHz = 5000000);

        ~SPI();

        void ReadWrite(const void *tx, void *rx, size_t length) const;
    };
}