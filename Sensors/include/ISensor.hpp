#pragma once
#include <mutex>


class ISensor {
protected:

	std::mutex _sensorMutex ;

public:

	//virtual bool Initialize() = 0;

	virtual bool ReadData() = 0;

	virtual bool Reload() = 0;

};