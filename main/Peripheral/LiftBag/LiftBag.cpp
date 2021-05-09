#include "LiftBag.hpp"


LiftBag::LiftBag(const UART *uart) :
        IPeripheral(IPeripheral::UARTBus, IPeripheral::Write),
        _flag(false),
        _uart(uart) {}

void LiftBag::SetData(bool flag) {
    std::lock_guard<std::mutex> mutex(this->_dataMutex);
    this->_flag = flag;
}

bool LiftBag::ReadData() {
    return false;
}

bool LiftBag::WriteData() {
    //ToDo: отправка сообщений
    uint8_t msg = this->_flag ? 0xFF : 0x00;
    //return _uart.recv(&msg, 1) == 1;

    return false;
}

bool LiftBag::Reload() {
    return false;
}

