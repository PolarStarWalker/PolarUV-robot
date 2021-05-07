#ifndef ROV_LIFTBAG_HPP
#define ROV_LIFTBAG_HPP

#include "../Interfaces/IPeripheral.hpp"

class LiftBag : protected IPeripheral {
private:
    bool _flag;



public:

    LiftBag();
    void SetData(bool flag);


};


#endif //ROV_LIFTBAG_HPP
