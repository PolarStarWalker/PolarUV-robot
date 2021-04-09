#include "SPIService.hpp"

SPIService::SPIService(const char *SPIName) {
    this->_spiDescriptor = open(SPIName, O_RDWR);
}

SPIService::~SPIService() {
    if (this->_spiDescriptor != -1) close(this->_spiDescriptor);
}

void SPIService::Setup() const {
    ioctl(this->_spiDescriptor,SPI_IOC_RD_MAX_SPEED_HZ, 5000000);
    ioctl(this->_spiDescriptor, SPI_CPOL, 0);
    ioctl(this->_spiDescriptor, SPI_CPHA, 0);
    ioctl(this->_spiDescriptor, SPI_CS_HIGH, 0);

}

void SPIService::Read(void *array, size_t length){
    read(this->_spiDescriptor, array, length);
}

void SPIService::Write(const void *array, size_t length) {
    write(this->_spiDescriptor, array, length);
}
