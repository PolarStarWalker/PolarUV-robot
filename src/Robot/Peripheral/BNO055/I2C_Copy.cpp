#include <cstdint>
#include <iostream>
#include "../../DataProtocols/DataTransmissions/I2C/I2C.hpp"

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

    if (ioctl(this->_i2CDescriptor, I2C_RDWR, &data) < 0) {
        std::cout << "Ошибка при чтении данных" << std::endl;
    }
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

    if (ioctl(this->_i2CDescriptor, I2C_RDWR, &data) < 0) {
        std::cout << "Ошибка при чтении данных" << std::endl;
    }
}

uint8_t I2C::ReadByte(__u16 slaveAddress, __u8 slaveRegister) const {
    __u8 outbuf[1], inbuf[1];
    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data msgset[1];

    msgs[0].addr = slaveAddress;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = outbuf;

    msgs[1].addr = slaveAddress;
    msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
    msgs[1].len = 1;
    msgs[1].buf = inbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 2;

    outbuf[0] = slaveRegister;

    inbuf[0] = 0;

    if (ioctl(this->_i2CDescriptor, I2C_RDWR, &msgset) < 0) {
        std::cout << "Ошибка при чтении данных" << std::endl;
    }

    return inbuf[0];
}

bool I2C::Write(__u16 slaveAddress,
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

    if (ioctl(this->_i2CDescriptor, I2C_RDWR, &data) < 0) {
        std::cout << "Ошибка при записи данных" << std::endl;
        return false;
    } else {
        return true;
    }
}

bool I2C::Write(__u16 slaveAddress,
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

    if (ioctl(this->_i2CDescriptor, I2C_RDWR, &data) < 0) {
        std::cout << "Ошибка при записи данных" << std::endl;
        return false;
    } else {
        return true;
    }
}

bool I2C::WriteByte(__u16 slaveAddress, __u8 slaveRegister, __u8 buffer) const {
    __u8 outbuf[2];

    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    outbuf[0] = slaveRegister;
    outbuf[1] = buffer;

    msgs[0].addr = slaveAddress;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(this->_i2CDescriptor, I2C_RDWR, &msgset) < 0) {
        perror("ioctl(I2C_RDWR) in i2c_write");
        return false;
    } else {
        return true;
    }
}
