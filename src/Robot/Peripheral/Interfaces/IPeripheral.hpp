#ifndef ROV_IPERIFERAL_HPP
#define ROV_IPERIFERAL_HPP
#include <mutex>
#include "../../DataTransmissions/I2C/I2C.hpp"
#include "../../DataTransmissions/SPI/SPI.hpp"
#include "../../DataTransmissions/UART/UART.hpp"

class IPeripheral {
protected:

    size_t _period = 0;

    std::mutex _dataMutex;

    bool _status = false;

    enum BusTypeEnum : int8_t {
        UninitializedBus = -1,
        I2CBus = 0,
        UARTBus = 1,
        SPIBus = 2
    } const _busType = UninitializedBus;

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

#endif