#pragma once
#include "ISensor.hpp"
#include "MS5837Data.hpp"

class MS5837 : ISensor, MS5837Registers {
private:

	MS5837Data* _data;

public:

	MS5837(uint8_t id);
	~MS5837();

	//bool Initialie() { return true;	}

	bool ReadData() { return true; }

	bool Reload() { return true; }
	
	uint8_t GetId() { return this->_id; }
};
