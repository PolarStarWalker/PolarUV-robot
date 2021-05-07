#pragma once

#include <thread>
#include <mutex>

#include "../../Peripheral/interfaces/IPeripheral.hpp"



class SensorsHandlerService {
private:
	std::mutex _arrayMutex;
	
	IPeripheral** _sensorsArray;
	size_t _sensorsNumber = 0;

public:

	void AddSensor(IPeripheral*);

};