#include <iostream>
#include "./SPI/SPI.hpp"

using namespace DataTransmissions;

SPI::SPI(const char *address, u_int32_t speedHz)
        : spifd_(open(address, O_RDWR)),
          _speed(speedHz){}

SPI::~SPI() {
    if (spifd_ != -1) close(this->spifd_);
}

void SPI::ReadWrite(const void *tx, void *rx, size_t length) const {

    struct spi_ioc_transfer message{};

    message.bits_per_word = _bits;
    message.cs_change = 0;
    message.len = length;
    message.rx_buf = (uint64_t) rx;
    message.tx_buf = (uint64_t) tx;
    message.speed_hz = _speed;
    message.delay_usecs = _delay;

    ioctl(spifd_, SPI_IOC_MESSAGE(1), &message);
}

