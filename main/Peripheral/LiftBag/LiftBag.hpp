#ifndef ROV_LIFTBAG_HPP
#define ROV_LIFTBAG_HPP

#include "../Interfaces/IPeripheral.hpp"
#include "../../DataProtocols/UART/UART.hpp"

class LiftBag : protected IPeripheral {
private:
    const UART *_uart;

    bool _flag;

    bool ReadData() final;

    bool WriteData() final;

    bool Reload() final;

public:

    explicit LiftBag(const UART *uart);

    void SetData(bool flag);
};


#endif //ROV_LIFTBAG_HPP
