#include "./PeripheralHandler/PeripheralHandler.hpp"

PeripheralHandler::PeripheralHandler(const char *i2c, size_t delay_us) :
        _i2c(i2c),
        _delay_us(delay_us) {}

bool PeripheralHandler::AddI2CSensor(II2CPeripheral *newSensor) const {
    _i2cMutex.lock();
    bool isInit = newSensor->Init(&_i2c);
    _i2cMutex.unlock();

    if (!isInit)
        return false;

    _i2cSensorsMutex.lock();
    _i2cPeripherals.emplace_back(newSensor);
    _i2cSensorsMutex.unlock();

    return true;
}

void PeripheralHandler::StartAsync() const{
    std::thread peripheralThread(&PeripheralHandler::Start, this);
    peripheralThread.detach();
}

[[noreturn]]
void PeripheralHandler::Start() const{
    for (;;) {

        _i2cSensorsMutex.lock_shared();
        std::list<I2CSensorsContext> bufer = _i2cPeripherals;
        _i2cSensorsMutex.unlock_shared();

        for (I2CSensorsContext peripheralContext : bufer) {
            _i2cSensorsMutex.lock();
            peripheralContext.IsOnline = peripheralContext.I2CPeripheral->ReadData();
            _i2cSensorsMutex.unlock();
        }

        usleep(_delay_us);
    }
}

