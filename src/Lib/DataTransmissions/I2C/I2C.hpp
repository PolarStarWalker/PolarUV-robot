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
#include <string_view>

class I2C {

private:
    int i2CDescriptor_ = -1;

public:
    explicit I2C(std::string_view &i2c_path);

    ~I2C();

    void Read(__u16 slaveAddress,
              const __u8 *slaveRegister,
              size_t addressLength,
              __u8 *buffer,
              size_t bufferLength) const;

    void Read(__u16 slaveAddress,
              __u8 *buffer,
              size_t bufferLength) const;

    uint8_t ReadByteFromRegister(__u16 slaveAddress,
                                 __u8 slaveRegister) const;

    ///Note не работает
    bool Write(__u16 slaveAddress,
               const __u8 *slaveRegister,
               size_t addressLength,
               const __u8 *buffer,
               size_t bufferLength) const;

    ///Note не работает
    bool Write(__u16 slaveAddress,
               const __u8 *buffer,
               size_t bufferLength) const;

    bool WriteByteToRegister(__u16 slaveAddress,
                             __u8 slaveRegister,
                             __u8 byte) const;

    bool WriteByte(__u16 slaveAddress,
                   __u8 byte) const;
};

#endif
