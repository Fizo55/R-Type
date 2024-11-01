#include "engineGraphicRay.hpp"
#include <unistd.h>

using namespace grw;

clock::clock()
    : _oldTime(static_cast<unsigned long>(GetTime() * 1000)) // Initialize with milliseconds from GetTime()
{
}

/**
 * @brief Get the time since the last call to this function
 */
unsigned long clock::tick(int fps) {
    unsigned long currentTime = static_cast<unsigned long>(GetTime() * 1000);  // Current time in milliseconds
    unsigned long delta = currentTime - this->_oldTime;  // Time difference since last tick
    long double executionTime;
    long double sleepTime;

    if (fps == -1) {
        this->_oldTime = currentTime;
        return delta;
    }

    executionTime = 1000.0 / fps;  // Target frame time in milliseconds
    sleepTime = executionTime - delta;

    if (sleepTime > 0.5) {
        sleep(static_cast<int>(std::round(sleepTime)));  // Sleep in milliseconds
    }
    this->_oldTime = static_cast<unsigned long>(GetTime() * 1000);  // Update _oldTime
    return (sleepTime > 0.0 ? delta + sleepTime : delta);
}

