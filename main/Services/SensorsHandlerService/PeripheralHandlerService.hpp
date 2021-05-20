#pragma once

#include <thread>
#include <mutex>
#include <list>
#include "../../Peripheral/Interfaces/IPeripheral.hpp"



class PeripheralHandlerService {
private:
	std::mutex _arrayMutex;
	
	std::list<IPeripheral*> _peripherals;
	size_t _sensorsNumber = 0;

	

public:

	void AddSensor(IPeripheral*);

	void Start();

};