#include "./PeripheralHandler/PeripheralHandler.hpp"


bool PeripheralHandler::AddI2CSensor(II2CPeripheral *newSensor) {
    this->_i2cMutex.lock();
    bool isInit = newSensor->Init(&this->_i2c);
    this->_i2cMutex.unlock();

    if (!isInit)
        return false;

    this->_i2cSensorsMutex.lock();
    this->_i2cPeripherals.emplace_back(newSensor);
    this->_i2cSensorsMutex.unlock();

    return true;
}

PeripheralHandler::PeripheralHandler(const char *i2c, size_t delay_us) :
        _i2c(i2c),
        _delay_us(delay_us) {
    usleep(1000 * 1000);
}

[[noreturn]]
void PeripheralHandler::Start() {
    for (;;) {
        for (I2CSensorsContext peripheralContext : this->_i2cPeripherals) {
            this->_i2cSensorsMutex.lock();
            peripheralContext.IsOnline = peripheralContext.I2CPeripheral->ReadData();
            this->_i2cSensorsMutex.unlock();
        }
        usleep(_delay_us);
    }
}

void PeripheralHandler::StartAsync() {
    std::thread peripheralThread(&PeripheralHandler::Start, this);
    peripheralThread.detach();
}
