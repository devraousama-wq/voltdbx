#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace voltdbx::concurrency {

class ThreadPool {
public:
    explicit ThreadPool(std::size_t worker_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void enqueue(std::function<void()> task);
    void shutdown();
    std::size_t pending() const;
    std::size_t workers() const { return workers_.size(); }

private:
    void worker_loop();

    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> tasks_;
    std::vector<std::thread> workers_;
    bool stopping_ = false;
};

}
