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

public:
    explicit SPIService(const char* SPIName);

    void Setup() const;

    void Read(void* array, size_t length);
    void Write(const void* array, size_t length);

    ~SPIService();
};