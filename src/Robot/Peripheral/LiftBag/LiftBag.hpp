#ifndef ROV_LIFTBAG_HPP
#define ROV_LIFTBAG_HPP

#include "../Interfaces/II2CPeripheral.hpp"
#include "../../DataTransmissions/UART/UART.hpp"

class LiftBag : protected II2CPeripheral {
private:
    const UART *_uart;

    bool _flag;

    bool ReadData() final;

    bool Reload() final;

public:

    explicit LiftBag(const UART *uart);

    void SetData(bool flag);
};


#endif //ROV_LIFTBAG_HPP
