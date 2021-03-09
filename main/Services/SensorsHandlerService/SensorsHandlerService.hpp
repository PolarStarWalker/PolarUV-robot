#pragma once

#include <thread>
#include <mutex>

#include "../../Sensors/interfaces/ISensors.hpp"



class SensorsHandlerService {
private:
	std::mutex _arrayMutex;
	
	ISensors** _sensorsArray;
	size_t _sensorsNumber = 0;

public:

	void AddSensor(ISensors*);

};