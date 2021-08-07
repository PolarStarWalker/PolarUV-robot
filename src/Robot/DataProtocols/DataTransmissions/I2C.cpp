#include <cstdint>
#include "I2C/I2C.hpp"

I2C::I2C(const char *address) {
    this->_i2CDescriptor = open(address, O_RDWR);
}

I2C::~I2C() {
    close(this->_i2CDescriptor);
}

void I2C::Read(__u16 slaveAddress,
               const uint8_t *slaveRegister,
               size_t addressLength,
               __u8 *buffer,
               size_t bufferLength)
const {

    struct i2c_rdwr_ioctl_data data{};
    struct i2c_msg messages[2];

    messages[0].addr = slaveAddress;
    messages[0].flags = I2C_SMBUS_WRITE;
    messages[0].len = addressLength;
    messages[0].buf = (__u8 *) slaveRegister;

    messages[1].addr = slaveAddress;
    messages[1].flags = I2C_SMBUS_READ;
    messages[1].len = bufferLength;
    messages[1].buf = buffer;

    data.msgs = messages;
    data.nmsgs = 2;

    ioctl(this->_i2CDescriptor, I2C_RDWR, &data);
}

void I2C::Read(__u16 slaveAddress,
               __u8 *buffer,
               size_t bufferLength)
const {
    struct i2c_rdwr_ioctl_data data{};
    struct i2c_msg messages{};

    messages.addr = slaveAddress;
    messages.flags = I2C_SMBUS_READ;
    messages.len = bufferLength;
    messages.buf = buffer;

    data.msgs = &messages;
    data.nmsgs = 2;

    ioctl(this->_i2CDescriptor, I2C_RDWR, &data);
}

void I2C::Write(__u16 slaveAddress,
                const __u8 *slaveRegister,
                size_t addressLength,
                const __u8 *buffer,
                size_t bufferLength)
const {

    struct i2c_rdwr_ioctl_data data{};
    struct i2c_msg messages[2];

    messages[0].addr = slaveAddress;
    messages[0].flags = I2C_SMBUS_WRITE;
    messages[0].len = addressLength;
    messages[0].buf = (__u8 *) slaveRegister;

    messages[1].addr = slaveAddress;
    messages[1].flags = I2C_SMBUS_WRITE;
    messages[1].len = bufferLength;
    messages[1].buf = (__u8 *) buffer;

    data.msgs = messages;
    data.nmsgs = 2;

    ioctl(this->_i2CDescriptor, I2C_RDWR, &data);
}

void I2C::Write(__u16 slaveAddress,
                const __u8 *buffer,
                size_t bufferLength)
const {
    struct i2c_rdwr_ioctl_data data{};
    struct i2c_msg messages{};

    messages.addr = slaveAddress;
    messages.flags = I2C_SMBUS_WRITE;
    messages.len = bufferLength;
    messages.buf = (__u8 *) buffer;

    data.msgs = &messages;
    data.nmsgs = 2;

    ioctl(this->_i2CDescriptor, I2C_RDWR, &data);
}
