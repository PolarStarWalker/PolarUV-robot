#ifndef ROV_I2C_HPP
#define ROV_I2C_HPP

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdint>

//close()
#include <unistd.h>

//ioctl()
#include <sys/ioctl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

class I2C {

private:
    int _i2CDescriptor = -1;

public:
    explicit I2C(const char *address);

    ~I2C();

    void Read(__u16 slaveAddress,
              const __u8 *slaveRegister,
              size_t addressLength,
              __u8 *buffer,
              size_t bufferLength) const;

    void Read(__u16 slaveAddress,
              __u8 *buffer,
              size_t bufferLength) const;

    uint8_t ReadByte(__u16 slaveAddress,
                     __u8 slaveRegister) const;

    bool Write(__u16 slaveAddress,
               const __u8 *slaveRegister,
               size_t addressLength,
               const __u8 *buffer,
               size_t bufferLength) const;

    bool Write(__u16 slaveAddress,
               const __u8 *buffer,
               size_t bufferLength) const;

    bool WriteByte(__u16 slaveAddress,
                   __u8 slaveRegister,
                   __u8 buffer) const;
};


#endif //ROV_I2C_HPP
