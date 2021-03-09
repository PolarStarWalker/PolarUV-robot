#pragma once
#include <mutex>


class ISensors {
protected:

	bool _onlineStatus = false;

	std::mutex _dataMutex ;

public:

	//virtual bool Initialize() = 0;

	virtual bool ReadData() = 0;

	virtual bool Reload() = 0;

};