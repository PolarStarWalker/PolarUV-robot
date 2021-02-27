#include "MS5837.hpp"


MS5837::MS5837(uint8_t id) {
	this->_data = new MS5837Data;
	this->_address = id;
}

MS5837::~MS5837() { delete this->_data; }

bool MS5837::Initialize() {


	this->_onlineStatus = true;
	return true;
}


bool MS5837::ReadData() {
	std::lock_guard<std::mutex>(this->_dataMutex);

	///TODO тут крч будет реализация чтения данных по I2C шине

	double depth = 1.0f;

	this->_data->depth = depth;
	return true;
}

bool MS5837::Reload() {
	std::lock_guard<std::mutex>(this->_dataMutex);


	///TODO тут будет реализация перезагрузки датчика
}