#ifndef ROBOT_CORUITINE_HPP
#define ROBOT_CORUITINE_HPP

#include <coroutine>
#include "./Event.hpp"

struct SensorTask {

    struct promise_type;

    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {

        //всегда такой
        auto get_return_object() noexcept { return coro_handle::from_promise(*this); }

        static inline auto initial_suspend() noexcept { return std::suspend_always(); }

        static inline auto final_suspend() noexcept { return std::suspend_always(); }

        void return_void() {}

        static inline void unhandled_exception() { std::terminate(); }

        auto yield_value(bool type) noexcept {
            isActive = type;
            return std::suspend_never();
        }

        auto await_transform(const itimerspec &timespec) noexcept {
            timespec_ = timespec;
            return std::suspend_always();
        }

        itimerspec timespec_{{0, 0},
                             {0, 0}};
        bool isActive = false;
    };

    std::pair<itimerspec, bool> operator()() {
        if (!handle_.done()) handle_.resume();
        return {handle_.promise().timespec_, handle_.promise().isActive};
    }

    SensorTask(coro_handle handle) : handle_(handle) {}

    SensorTask(const SensorTask &) = delete;

    SensorTask(SensorTask &&task) noexcept: handle_(task.handle_) { task.handle_ = nullptr; }

    ~SensorTask() { if (handle_) handle_.destroy(); }

protected:
    coro_handle handle_;
};

#endif