#include "PeripheralHandlerService.hpp"

void PeripheralHandlerService::AddSensor(IPeripheral* newSensor)
{
    this->_peripherals.push_back(newSensor);
}
