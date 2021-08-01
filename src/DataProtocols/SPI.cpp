#include <iostream>
#include "./SPI/SPI.hpp"

SPI::SPI(const char *address, u_int32_t speedHz) {
    this->_spiDescriptor = open(address, O_RDWR);
    this->_speed = speedHz;
}

SPI::~SPI() {
    if (this->_spiDescriptor != -1) close(this->_spiDescriptor);
}

void SPI::ReadWrite(const void *tx, void *rx, size_t length) const {

    struct spi_ioc_transfer message{};

    message.bits_per_word = this->_bits;
    message.cs_change = 0;
    message.len = length;
    message.rx_buf = (uint64_t) rx;
    message.tx_buf = (uint64_t) tx;
    message.speed_hz = this->_speed;
    message.delay_usecs = this->_delay;

    ioctl(this->_spiDescriptor, SPI_IOC_MESSAGE(1), &message);
}

