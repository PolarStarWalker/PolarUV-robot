#ifndef ROBOT_EVENT_HPP
#define ROBOT_EVENT_HPP
#include <sys/timerfd.h>
#include <sys/epoll.h>

class TimerType {
    int timerfd_;
public:
    TimerType();

    ~TimerType();

    bool operator==(int fd) const { return timerfd_ == fd; }

    explicit operator int() const { return timerfd_; }

    static itimerspec SleepFor_us(ssize_t us);
    static itimerspec SleepFor_ms(ssize_t ms);

    bool SetTimer(const itimerspec &time) const;
};


class EventTracker {
    int epollfd_;
public:

    explicit EventTracker(int maxEvents);

    ~EventTracker();

    bool TrackEvent(const TimerType &timer, uint32_t events) const;

    template<size_t MAX_EVENTS>
    std::pair<std::array<epoll_event, MAX_EVENTS>, int> Listen(ssize_t timeout_ms) const {
        std::array<epoll_event, MAX_EVENTS> epollEvents;

        int events = epoll_wait(epollfd_, epollEvents.begin(), MAX_EVENTS, timeout_ms);

        return {epollEvents, events};
    }
};

#endif
