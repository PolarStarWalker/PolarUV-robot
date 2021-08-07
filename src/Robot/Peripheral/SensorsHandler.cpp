#include "./SensorsHandler/SensorsHandler.hpp"

void SensorsHandler::AddSensor(IPeripheral* newSensor)
{
    this->_peripherals.push_back(newSensor);
}
