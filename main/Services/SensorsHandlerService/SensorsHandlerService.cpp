#include "SensorsHandlerService.hpp"

void SensorsHandlerService::AddSensor(IPeripheral* newSensor)
{
	IPeripheral** tmpSensorsArray = new IPeripheral* [_sensorsNumber + 1];
	
	for (size_t i = 0; i < _sensorsNumber; i++) {
		tmpSensorsArray[i] = this->_sensorsArray[i];
	}

	tmpSensorsArray[_sensorsNumber+1] = newSensor;

	this->_arrayMutex.lock();
	_sensorsNumber += 1;
	std::swap(tmpSensorsArray, this->_sensorsArray);
	this->_arrayMutex.unlock();

	delete[]  tmpSensorsArray;
}
