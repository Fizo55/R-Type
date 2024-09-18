#include "engineGraphic.hpp"

using namespace grw;

clock::clock()
    : _oldTime(SDL_GetTicks64())
{

}

unsigned long clock::tick(int fps) {
    long double executionTime;
    unsigned long delta = SDL_GetTicks64() - this->_oldTime;
    long double sleepTime;

    if (fps == -1) {
        this->_oldTime = SDL_GetTicks64();
        return (delta);
    }

    executionTime = 1000.0 / fps;
    sleepTime = executionTime - delta;

    if (sleepTime > 0.5)
        SDL_Delay((unsigned long)round(sleepTime));
    this->_oldTime = SDL_GetTicks64();
    return (sleepTime > 0.0 ? delta + sleepTime : delta);
}
