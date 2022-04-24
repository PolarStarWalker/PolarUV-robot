#include "Peripheral/PeripheralHandler/SensorHandler.hpp"

#include <iostream>
#include <ranges>
#include <algorithm>
#include <utility>
#include <variant>

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <Peripheral/PeripheralHandler/Event.hpp>

TimerType::TimerType() : timerfd_(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK)) {}

TimerType::~TimerType() {
    close(timerfd_);
}

itimerspec TimerType::SleepFor_us(ssize_t us) {
    return {{0, 0},
            {0, us * 1000}};
}

itimerspec TimerType::SleepFor_ms(ssize_t ms) {
    return {{0, 0},
            {0, ms * 1000 * 1000}};
}

bool TimerType::SetTimer(const itimerspec &time) const {

    bool success = timerfd_settime(timerfd_, 0, &time, nullptr) == 0;

    return success;
}

EventTracker::EventTracker(int maxEvents) : epollfd_(epoll_create(maxEvents)) {}

EventTracker::~EventTracker() {
    close(epollfd_);
}

bool EventTracker::TrackEvent(const TimerType &timer, uint32_t events) const {

    epoll_event event{};
    event.events = events;
    event.data.fd = (int) timer;

    int res = epoll_ctl(epollfd_, EPOLL_CTL_ADD, (int) timer, &event);

    return res == 0;
}


SensorContext::SensorContext(std::shared_ptr<ISensor> sensors, SensorTask &&init, SensorTask &&readData) :
        Timer(),
        State(Offline),
        Sensor(std::move(sensors)),
        Init(std::move(init)),
        ReadData(std::move(readData)) {
    auto time = Timer.SleepFor_ms(10);
    Timer.SetTimer(time);
}

SensorHandler::SensorHandler(std::string_view i2c_path) :
        i2c_(i2c_path),
        eventTracker_(MAX_SENSORS),
        notDone_(true),
        thread_() {}

bool SensorHandler::RegisterSensor(const std::shared_ptr<ISensor> &newSensor,
                                   SensorTask &&init,
                                   SensorTask &&readData) {

    auto &context = sensors_.emplace_front(newSensor, std::move(init), std::move(readData));

    if (eventTracker_.TrackEvent(context.Timer, EPOLLIN))
        return true;

    sensors_.pop_front();

    return false;
}

void SensorHandler::StartAsync() const {
    thread_ = std::thread(&SensorHandler::Start, this);
}

void SensorHandler::Start() const {

    int timerfd;

    auto comparator = [&](const SensorContext &context) { return context.Timer == timerfd; };

    while (notDone_.load()) {

        auto [epollEvents, events] = eventTracker_.Listen<MAX_SENSORS>(MAX_TIMEOUT_MS);

        if (events <= 0)
            continue;

//        auto begin = std::chrono::system_clock::now();

        for (size_t i = 0; i < events; ++i) {

            timerfd = epollEvents[i].data.fd;

            auto context = std::ranges::find_if(sensors_, comparator);

            if (context == sensors_.end())
                continue;

            // to reset the timer overflow amount
            uint64_t ticNumber;
            ssize_t size = read(timerfd, (char *) &ticNumber, sizeof(uint64_t));

            switch (context->State) {
                case SensorContext::Online: {
                    auto [time, isOnline] = context->ReadData();
                    context->Timer.SetTimer(time);
                    if (!isOnline)
                        context->State = SensorContext::Offline;
                    break;
                }
                case SensorContext::Offline: {
                    auto [time, isOnline] = context->Init();
                    context->Timer.SetTimer(time);
                    if (isOnline)
                        context->State = SensorContext::Online;
                    break;
                }
                default:
                    throw std::exception();
            }
        }

//        auto end = std::chrono::system_clock::now();

//        std::cout << "Execution time: "
//                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()
//                  << "[ns]\n";

    }
}

SensorHandler::~SensorHandler() {
    notDone_.exchange(false);
    thread_.join();
}
