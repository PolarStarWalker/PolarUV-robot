#include "MS5837.hpp"

MS5837::MS5837(uint8_t address): IPeripheral(IPeripheral::I2CBus, IPeripheral::Read){
	this->_address = address;
}

bool MS5837::Initialize() {


	this->_status = true;
	return true;
}


bool MS5837::ReadData() {
	std::lock_guard<std::mutex> mutex(this->_dataMutex);

    ///TODO обработка датчика

	double depth = 1.0f;

	this->_data.depth = depth;
	return true;
}


bool MS5837::Reload(){
	std::lock_guard<std::mutex> mutex(this->_dataMutex);


	///TODO обработка датчика
	return true;
}

MS5837Data MS5837::GetDepth() {
    std::lock_guard<std::mutex> mutex(this->_dataMutex);
    return this->_data;
}

bool MS5837::WriteData() {
    return false;
}

