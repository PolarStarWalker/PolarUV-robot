#pragma once
#include "../Interfaces/IData.hpp"

struct MS5837Data : IData{
    double pressure = 0.0;
    double temperature = 0.0;
	double depth = 0.0;
};


class MS5837Registers {
protected:
	uint8_t _address = 0;

public:

};