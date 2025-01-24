#include <math.h>
#include <chrono>
#include <thread>

#include <gameServer.hpp>

Clock::Clock()
    : _oldTime(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count()), _lastDT(0)
{

}


/**
 * @brief Get the time since the last call to this function
 */
unsigned long Clock::tick(int fps) {
    long double executionTime;
    unsigned long delta = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() - this->_oldTime;
    long double sleepTime;

    if (fps == -1) {
        this->_oldTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
        this->_lastDT = (delta);
        return (this->_lastDT);
    }

    executionTime = 1000.0 / fps;
    sleepTime = executionTime - delta;

    if (sleepTime > 0.5)
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)std::round(sleepTime)));
    this->_oldTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    this->_lastDT = (sleepTime > 0.0 ? delta + sleepTime : delta);
    return (this->_lastDT);
}

unsigned long Clock::lastDT(void) {
    return (this->_lastDT);
}