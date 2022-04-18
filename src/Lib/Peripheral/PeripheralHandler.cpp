#include "./PeripheralHandler/PeripheralHandler.hpp"

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>

constexpr size_t MAGIC_NUMBER = true;

I2CSensorsContext::I2CSensorsContext(II2CPeripheral *i2cPeripheral) :
        timerfd(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK)),
        IsOnline(false),
        WaitTransmissions(false),
        I2CPeripheral(i2cPeripheral) {

    long int delay = i2cPeripheral->DelayUs();
    timespec spec{0, delay};
    itimerspec ispec{spec, spec};

    timerfd_settime(timerfd, 0, &ispec, nullptr);
}

I2CSensorsContext::~I2CSensorsContext(){
    close(timerfd);
}

PeripheralHandler::PeripheralHandler(std::string_view i2c_path, size_t delay_us) :
        _i2c(i2c_path),
        delay_us_(delay_us),
        epollfd(epoll_create(MAGIC_NUMBER)) {}

bool PeripheralHandler::AddI2CSensor(II2CPeripheral &newSensor) const {

    {
        std::lock_guard guard(i2cMutex_);
        bool isInit = newSensor.Init(&_i2c);

        if (!isInit)
            return false;

    }

    std::lock_guard guard(peripheralMutex_);
    i2cPeripherals_.emplace_back(&newSensor);



    return true;
}

void PeripheralHandler::StartAsync() const {
    std::thread peripheralThread(&PeripheralHandler::Start, this);
    peripheralThread.detach();
}

void PeripheralHandler::Start() const {
    for (;;) {

        size_t delay_us;
        std::list<I2CSensorsContext> buffer;

        {
            std::lock_guard guard(peripheralMutex_);
            buffer.splice(buffer.begin(), i2cPeripherals_);
            delay_us = delay_us_;
        }

        for (I2CSensorsContext peripheralContext: buffer) {
            std::lock_guard guard(i2cMutex_);
            peripheralContext.IsOnline = peripheralContext.I2CPeripheral->ReadData();
        }

        {
            std::lock_guard guard(peripheralMutex_);
            i2cPeripherals_.splice(i2cPeripherals_.end(), buffer);
        }

        usleep(delay_us);
    }
}

PeripheralHandler::~PeripheralHandler() {
    close(epollfd);
}

