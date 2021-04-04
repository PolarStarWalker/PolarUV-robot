#pragma once
//open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//close()
#include <unistd.h>

//ioctl()
#include <sys/ioctl.h>


class SPIService {
private:
    const char *_spiName = nullptr;
    int _spiDescriptor = -1;

public:
    explicit SPIService(const char* SPIName);

    ~SPIService();
};