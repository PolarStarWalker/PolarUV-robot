#include "./PeripheralHandler/PeripheralHandler.hpp"

#include <ranges>
#include <algorithm>

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>

constexpr size_t MAGIC_NUMBER = true;

I2CSensorsContext::I2CSensorsContext(II2CPeripheral *i2cPeripheral) :
        Timerfd(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK)),
        IsOnline(false),
        WaitTransmissions(false),
        I2CPeripheral(i2cPeripheral) {

    long int delay = i2cPeripheral->DelayUs();
    timespec spec{0, delay};
    itimerspec ispec{spec, spec};
    timerfd_settime(Timerfd, 0, &ispec, nullptr);
}

I2CSensorsContext::~I2CSensorsContext() {
    close(Timerfd);
}

PeripheralHandler::PeripheralHandler(std::string_view i2c_path) :
        i2c_(i2c_path),
        epollfd_(epoll_create(MAGIC_NUMBER)),
        thread_(){}

bool PeripheralHandler::AddI2CSensor(II2CPeripheral &newSensor) const {

    {
        std::lock_guard guard(i2cMutex_);
        bool isInit = newSensor.Init(&i2c_);

        if (!isInit)
            return false;

    }

    std::lock_guard guard(peripheralMutex_);
    I2CSensorsContext context(&newSensor);

    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = context.Timerfd;

    int res = epoll_ctl(epollfd_, EPOLL_CTL_ADD, context.Timerfd, &event);

    if (res == 0) {
        i2cPeripherals_.push_back(context);
        return true;
    }

    return false;
}

void PeripheralHandler::StartAsync() const {
    std::thread peripheralThread(&PeripheralHandler::Start, this);
}

void PeripheralHandler::Start() const {

    epoll_event epollEvents[MAX_SENSORS];

    int timerfd;

    auto comparator = [&](const I2CSensorsContext &context) { return context.Timerfd == timerfd; };

    while (notDone_) {

        int events = epoll_wait(epollfd_, epollEvents, MAX_SENSORS, MAX_TIMEOUT_MS);

        if (events < 0)
            continue;

        for (size_t i = 0; i < events; ++i) {

            timerfd = epollEvents[i].data.fd;

            std::_List_iterator<I2CSensorsContext> context;

            {
                std::unique_lock guard(peripheralMutex_);
                context = std::ranges::find_if(i2cPeripherals_, comparator);
            }

            if (context == i2cPeripherals_.end())
                continue;

            std::lock_guard guard(i2cMutex_);

            if(context->IsOnline)
                context->IsOnline = context->I2CPeripheral->ReadData();
            else
                context->IsOnline = context->I2CPeripheral->Reload();

        }
    }
}

PeripheralHandler::~PeripheralHandler() {
    close(epollfd_);
    notDone_.exchange(false);
    thread_.join();
}

