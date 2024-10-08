#ifndef ITASKSCHEDULER_HPP
#define ITASKSCHEDULER_HPP

#include <functional>

class ITaskScheduler {
public:
    virtual ~ITaskScheduler() = default;

    virtual void scheduleTask(const std::function<void()>& task) = 0;
    virtual void run() = 0;
};

#endif
