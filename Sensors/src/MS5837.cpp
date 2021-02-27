#include "MS5837.hpp"


MS5837::MS5837(uint8_t id) {
	this->_data = new MS5837Data;
	this->_id = id;
}

MS5837::~MS5837() { delete this->_data; }