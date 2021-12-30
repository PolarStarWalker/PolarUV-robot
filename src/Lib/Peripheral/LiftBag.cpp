#include "./LiftBag/LiftBag.hpp"


LiftBag::LiftBag(const UART *uart) :
        _flag(false),
        _uart(uart) {}

void LiftBag::SetData(bool flag) {
    this->_flag = flag;
}

bool LiftBag::ReadData() {
    return false;
}

bool LiftBag::Reload() {
    return false;
}

