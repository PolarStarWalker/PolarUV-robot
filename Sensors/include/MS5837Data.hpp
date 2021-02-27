#pragma once
#include "IData.hpp"

struct MS5837Data : IData{
	double depth = 0.0f;
};


class MS5837Registers {
protected:
	uint8_t _id = 0;

public:

};