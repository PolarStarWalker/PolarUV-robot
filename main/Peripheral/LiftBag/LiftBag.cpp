#include "LiftBag.hpp"


LiftBag::LiftBag() : IPeripheral(IPeripheral::UART, IPeripheral::Write), _flag(false){}

void LiftBag::SetData(bool flag) {
    std::lock_guard<std::mutex> mutex(this->_dataMutex);
    this->_flag = flag;
}

