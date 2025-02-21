#include "engineGraphicRay.hpp"
#include <thread>
#include <chrono>
#include <cmath>

using namespace grw;

clock::clock()
    : _oldTime(static_cast<unsigned long>(GetTime() * 1000)) // Initialize with milliseconds from GetTime()
{
}

/**
 * @brief Get the time since the last call to this function
 */
unsigned long clock::tick(int fps)
{
    unsigned long currentTime = static_cast<unsigned long>(GetTime() * 1000);
    unsigned long delta = currentTime - this->_oldTime;
    long double executionTime;
    long double sleepTime;

    if (fps == -1) {
        this->_oldTime = currentTime;
        return delta;
    }

    executionTime = 1000.0 / fps;
    sleepTime = executionTime - delta;

    if (sleepTime > 0.5) {
        std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(std::round(sleepTime * 1000))));
    }
    this->_oldTime = static_cast<unsigned long>(GetTime() * 1000);
    return (sleepTime > 0.0 ? delta + sleepTime : delta);
}
