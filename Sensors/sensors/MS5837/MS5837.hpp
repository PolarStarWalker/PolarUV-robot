#pragma once
#include "../ISensors.hpp"
#include "MS5837Data.hpp"

class MS5837 : ISensors, MS5837Registers {
private:

	MS5837Data* _data;

public:

	MS5837(uint8_t id);
	~MS5837();

	//from interface
	bool ReadData();
	bool Reload();

	//from sensor
	bool Initialize();
	uint8_t GetId() { return this->_address; }
};
