#include "engineGraphic.hpp"

#include <math.h>

using namespace grw;

clock::clock()
    : _oldTime(SDL_GetTicks64()), _lastDT(0)
{

}


/**
 * @brief Get the time since the last call to this function
 */
unsigned long clock::tick(int fps) {
    long double executionTime;
    unsigned long delta = SDL_GetTicks64() - this->_oldTime;
    long double sleepTime;

    if (fps == -1) {
        this->_oldTime = SDL_GetTicks64();
        this->_lastDT = (delta);
        return (this->_lastDT);
    }

    executionTime = 1000.0 / fps;
    sleepTime = executionTime - delta;

    if (sleepTime > 0.5)
        SDL_Delay((unsigned long)std::round(sleepTime));
    this->_oldTime = SDL_GetTicks64();
    this->_lastDT = (sleepTime > 0.0 ? delta + sleepTime : delta);
    return (this->_lastDT);
}

unsigned long clock::lastDT(void) {
    return (this->_lastDT);
}