#ifndef ROBOT_DEFAULT_HPP
#define ROBOT_DEFAULT_HPP
#include <coroutine>

template<typename T, typename C>
struct Expected{
    T Value;
    C Error;
};

template<typename Type = void>
struct Task;

template<>
struct Task<void> {

    struct promise_type;

    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {

        //всегда такой
        auto get_return_object()noexcept { return coro_handle::from_promise(*this); }

        static auto initial_suspend() noexcept { return std::suspend_always(); }

        static auto final_suspend() noexcept { return std::suspend_always(); }

        void return_void() {}

        static void unhandled_exception() { std::terminate(); }
    };

    bool operator()(){
        if (!handle_.done()) handle_.resume();
        return handle_.done();
    }

    Task(coro_handle handle) : handle_(handle) {}

    Task(const Task &) = delete;

    Task(Task &&task) noexcept: handle_(task.handle_) { task.handle_ = nullptr; }

    ~Task() { handle_.destroy(); }

protected:
    coro_handle handle_;
};


template<typename Type>
struct Task {

    struct promise_type;

    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {

        //всегда такой
        auto get_return_object() noexcept { return coro_handle::from_promise(*this); }

        static inline auto initial_suspend() noexcept { return std::suspend_always(); }

        static inline auto final_suspend() noexcept { return std::suspend_always(); }

        void return_void() {}

        static inline void unhandled_exception() { std::terminate(); }

        auto yield_value(Type type) {
            isActive = std::move(type);
            return std::suspend_always();
        }

        Type isActive;
    };

    Expected<Type, bool> operator()(){
        if (!handle_.done()) handle_.resume();
        return {handle_.promise().isActive , handle_.done()};
    }

    Task(coro_handle handle) : handle_(handle) {}

    Task(const Task &) = delete;

    Task(Task &&task) noexcept: handle_(task.handle_) { task.handle_ = nullptr; }

    ~Task() { handle_.destroy(); }

protected:
    coro_handle handle_;
};

#endif