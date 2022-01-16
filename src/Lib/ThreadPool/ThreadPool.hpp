#ifndef ROBOT_THREADPOOL_HPP
#define ROBOT_THREADPOOL_HPP

#include <type_traits>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

template<typename Type>
requires std::is_function_v<Type>
class ThreadPool {
public:

    explicit ThreadPool(size_t threads);

    ~ThreadPool();

    void AddTask(const Type &task);

private:

    [[noreturn]]
    void WorkerThread();

    Type PopTask();

    std::vector<std::thread> _threads;

    std::mutex _queueMutex;
    std::queue<Type> _queue;
    std::condition_variable _queueIsNotEmpty;
    std::atomic<bool> _isOnline;

};

template<typename Type>
requires std::is_function_v<Type>
ThreadPool<Type>::ThreadPool(size_t threads) : _threads(threads), _queue() {

    for (auto &&thread: _threads) {
        thread = std::thread(&ThreadPool<Type>::WorkerThread, this);
        thread.detach();
    }
}


template<typename Type>
requires std::is_function_v<Type>
void ThreadPool<Type>::WorkerThread() {
    while (_isOnline) {
        auto task = PopTask();
        task();
    }
}

template<typename Type>
requires std::is_function_v<Type>
void ThreadPool<Type>::AddTask(const Type &task) {
    std::lock_guard guard(_queueMutex);
    _queue.template emplace(task);
    _queueIsNotEmpty.notify_one();
}

template<typename Type>
requires std::is_function_v<Type>
Type ThreadPool<Type>::PopTask() {
    std::unique_lock guard(_queueMutex);
    _queueIsNotEmpty.wait(guard, [&]() { return _queue.empty(); });
    return _queue.pop();
}

template<typename Type>
requires std::is_function_v<Type>
ThreadPool<Type>::~ThreadPool() {

    _isOnline.exchange(false);

    for (auto &thread: _threads)
        thread.join();

}


#endif