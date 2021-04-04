#include "SPIService.hpp"

SPIService::SPIService(const char *SPIName) {
    this->_spiName = SPIName;
    this->_spiDescriptor = open(SPIName, O_RDWR);
}

SPIService::~SPIService() {
    if (this->_spiDescriptor != -1) close(this->_spiDescriptor);
}
