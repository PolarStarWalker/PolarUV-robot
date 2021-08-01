#include "./LiftBag/LiftBag.hpp"


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
    this->_dataMutex.lock();
    uint64_t msg = this->_flag ? 0xFFFFFFFFFFFFFFFF : 0x00000000000000;
    this->_dataMutex.unlock();

    return _uart->recv((uint8_t*) &msg, 1) == 1;
}

bool LiftBag::Reload() {
    return false;
}

