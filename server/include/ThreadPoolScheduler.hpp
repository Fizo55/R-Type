#ifndef THREADPOOLSCHEDULER_HPP
#define THREADPOOLSCHEDULER_HPP

#include "ITaskScheduler.hpp"
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPoolScheduler : public ITaskScheduler {
public:
    ThreadPoolScheduler(size_t threadCount);
    ~ThreadPoolScheduler();

    void scheduleTask(const std::function<void()>& task) override;
    void run() override;

private:
    void workerThread();

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasksMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};

#endif
