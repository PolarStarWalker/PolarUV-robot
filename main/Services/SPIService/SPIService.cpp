#include <iostream>
#include "SPIService.hpp"

SPIService::SPIService(const char *SPIName) {
    this->_spiDescriptor = open(SPIName, O_RDWR);
}

SPIService::~SPIService() {
    if (this->_spiDescriptor != -1) close(this->_spiDescriptor);

}

void SPIService::ReadWrite(const void *tx, void *rx, size_t length) const {

    struct spi_ioc_transfer message{};

    message.bits_per_word = this->bits;
    message.cs_change = 0;
    message.len = length;
    message.rx_buf = (uint64_t) rx;
    message.tx_buf = (uint64_t) tx;
    message.speed_hz = this->speed;
    message.delay_usecs = this->delay;

    ioctl(this->_spiDescriptor, SPI_IOC_MESSAGE(1), &message);
}

