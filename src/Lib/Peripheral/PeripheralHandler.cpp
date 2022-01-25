#include "./PeripheralHandler/PeripheralHandler.hpp"

PeripheralHandler::PeripheralHandler(std::string_view i2c_path, size_t delay_us) :
        _i2c(i2c_path),
        delay_us_(delay_us) {}

bool PeripheralHandler::AddI2CSensor(II2CPeripheral *newSensor) const {

    {
        std::lock_guard guard (i2cMutex_);
        bool isInit = newSensor->Init(&_i2c);

        if (!isInit)
            return false;

    }

    std::lock_guard guard(i2cPeripheralMutex_);
    i2cPeripherals_.emplace_back(newSensor);

    return true;
}

void PeripheralHandler::StartAsync() const{
    std::thread peripheralThread(&PeripheralHandler::Start, this);
    peripheralThread.detach();
}

void PeripheralHandler::Start() const{
    for (;;) {

        size_t delay_us;
        std::list<I2CSensorsContext> buffer;

        {
            std::lock_guard guard(i2cPeripheralMutex_);
            buffer.splice(buffer.begin(), i2cPeripherals_);
            delay_us = delay_us_;
        }

        for (I2CSensorsContext peripheralContext : buffer) {
            std::lock_guard guard(i2cMutex_);
            peripheralContext.IsOnline = peripheralContext.I2CPeripheral->ReadData();
        }

        {
            std::lock_guard guard(i2cPeripheralMutex_);
            i2cPeripherals_.splice(i2cPeripherals_.end(), buffer);
        }

        usleep(delay_us);
    }
}

