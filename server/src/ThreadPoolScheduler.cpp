#include "ThreadPoolScheduler.hpp"

ThreadPoolScheduler::ThreadPoolScheduler(size_t threadCount)
    : stop_(false)
{
    for (size_t i = 0; i < threadCount; ++i) {
        threads_.emplace_back(&ThreadPoolScheduler::workerThread, this);
    }
}

ThreadPoolScheduler::~ThreadPoolScheduler()
{
    stop_ = true;
    condition_.notify_all();
    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPoolScheduler::scheduleTask(const std::function<void()>& task)
{
    {
        std::unique_lock<std::mutex> lock(tasksMutex_);
        tasks_.push(task);
    }
    condition_.notify_one();
}

void ThreadPoolScheduler::run()
{
    // Useless for now
}

void ThreadPoolScheduler::workerThread()
{
    while (!stop_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(tasksMutex_);
            condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) {
                return;
            }
            task = tasks_.front();
            tasks_.pop();
        }
        task();
    }
}
