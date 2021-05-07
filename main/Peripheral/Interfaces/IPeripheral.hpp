#ifndef ROV_IPERIFERAL_HPP
#define ROV_IPERIFERAL_HPP

#include <mutex>

class IPeripheral {
protected:

    size_t _period = 0;

    std::mutex _dataMutex;

    bool _status = false;

    enum BusTypeEnum : int8_t {
        BusUninitialized = -1,
        I2C = 0,
        UART = 1,
        SPI = 3
    } const _busType = BusUninitialized;

    enum ActionTypeEnum : int8_t {
        ActionUninitialized = -1,
        Read = 0,
        Write = 1,
        ReadWrite = 2,
        WriteRead = 3
    } const _actionType = ActionUninitialized;

    IPeripheral(BusTypeEnum busType, ActionTypeEnum actionType) : _busType(busType), _actionType(actionType) {};

    virtual bool ReadData() = 0;

    virtual bool WriteData() = 0;

    virtual bool Reload() = 0;
};

#endif //ROV_IPERIFERAL_HPP