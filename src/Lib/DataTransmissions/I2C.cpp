#include <cstdint>
#include <iostream>
#include "I2C/I2C.hpp"

#ifdef DEBUGG
#define DEBUG_OUT(str) (std::cout << str << std::endl)
#else
#define DEBUG_OUT(str)
#endif

I2C::I2C(std::string_view &i2c_path):
    i2CDescriptor_(open(i2c_path.data(), O_RDWR)){}

I2C::~I2C() {
    close(i2CDescriptor_);
}

bool I2C::Read(__u16 slaveAddress,
               __u8 *buffer,
               size_t bufferLength){
    struct i2c_rdwr_ioctl_data data{};
    struct i2c_msg messages{};

    messages.addr = slaveAddress;
    messages.flags = I2C_SMBUS_READ;
    messages.len = bufferLength;
    messages.buf = buffer;

    data.msgs = &messages;
    data.nmsgs = 1;

    if (ioctl(i2CDescriptor_, I2C_RDWR, &data) < 0) {
        DEBUG_OUT("Ошибка при чтении данных");
        return false;
    }

    return true;
}

std::pair<uint8_t, bool> I2C::ReadByteFromRegister(__u16 slaveAddress, __u8 slaveRegister) {
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

    if (ioctl(i2CDescriptor_, I2C_RDWR, &msgset) < 0) {
        DEBUG_OUT("Ошибка при чтении данных");
        return {inbuf[0], false};
    }


    return {inbuf[0], true};
}


bool I2C::ReadFromRegister(__u16 slaveAddress, __u8 slaveRegister, __u8 *data, size_t size) {
    __u8 outbuf[1];
    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data msgset[1];

    msgs[0].addr = slaveAddress;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = outbuf;

    msgs[1].addr = slaveAddress;
    msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
    msgs[1].len = size;
    msgs[1].buf = data;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 2;

    outbuf[0] = slaveRegister;

    if (ioctl(i2CDescriptor_, I2C_RDWR, &msgset) < 0) {
        DEBUG_OUT("Ошибка при чтении данных");
        return false;
    }

    return true;
}

bool I2C::WriteByteToRegister(__u16 slaveAddress, __u8 slaveRegister, __u8 byte) {
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    __u8 outbuf[2];
    outbuf[0] = slaveRegister;
    outbuf[1] = byte;

    msgs[0].addr = slaveAddress;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(i2CDescriptor_, I2C_RDWR, &msgset) < 0) {
        DEBUG_OUT("Ошибка при записи данных");
        return false;
    }

    return true;
}

bool I2C::WriteByte(__u16 slaveAddress, __u8 byte) {
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    __u8 outbuf = byte;

    msgs[0].addr = slaveAddress;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = &outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(i2CDescriptor_, I2C_RDWR, &msgset) < 0) {
        DEBUG_OUT("Ошибка при записи данных");
        return false;
    }

    return true;
}
