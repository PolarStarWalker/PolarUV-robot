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
    int i2CDescriptor_;

public:
    explicit I2C(std::string_view &i2c_path);

    ~I2C();

    bool Read(__u16 slaveAddress, __u8 *buffer, size_t bufferLength);

    [[nodiscard]] std::pair<uint8_t, bool> ReadByteFromRegister(__u16 slaveAddress, __u8 slaveRegister);

    bool ReadFromRegister(__u16 slaveAddress, __u8 slaveRegister, __u8* buffer, size_t size);

    bool WriteByteToRegister(__u16 slaveAddress, __u8 slaveRegister, __u8 byte);

    bool WriteByte(__u16 slaveAddress, __u8 byte);
};

#endif
