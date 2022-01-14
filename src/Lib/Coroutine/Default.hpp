#ifndef ROBOT_DEFAULT_HPP
#define ROBOT_DEFAULT_HPP
#include <coroutine>

template<typename Type = void>
struct Task;

template<>
struct Task<void> {

    struct promise_type;

    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {

        //всегда такой
        auto get_return_object()noexcept { return coro_handle::from_promise(*this); }

        static auto initial_suspend() noexcept { return std::suspend_never(); }

        static auto final_suspend() noexcept { return std::suspend_always(); }

        void return_void() {}

        static void unhandled_exception() { std::terminate(); }

    };

    bool resume() {
        if (!_handle.done()) _handle.resume();
        return _handle.done();
    }

    Task(coro_handle handle) : _handle(handle) {}

    Task(const Task &) = delete;

    Task(Task &&task) noexcept: _handle(task._handle) { task._handle = nullptr; }

    ~Task() { _handle.destroy(); }

protected:
    coro_handle _handle;
};


template<typename Type>
struct Task {

    struct promise_type;

    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {

        //всегда такой
        auto get_return_object() noexcept { return coro_handle::from_promise(*this); }

        static inline auto initial_suspend() noexcept { return std::suspend_never(); }

        static inline auto final_suspend() noexcept { return std::suspend_always(); }

        void return_void() {}

        static inline void unhandled_exception() { std::terminate(); }

        auto yield_value(Type type) {
            current_value = std::move(type);
            return std::suspend_always{};
        }

        Type current_value;
    };

    bool resume() {
        if (!_handle.done()) _handle.resume();
        return _handle.done();
    }

    Type current_value() { return _handle.promise().current_value; }

    Task(coro_handle handle) : _handle(handle) {}

    Task(const Task &) = delete;

    Task(Task &&task) noexcept: _handle(task._handle) { task._handle = nullptr; }

    ~Task() { _handle.destroy(); }

protected:
    coro_handle _handle;
};

#endif