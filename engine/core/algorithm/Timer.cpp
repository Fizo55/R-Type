/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Timer
*/

#include "Timer.hpp"

Timer::Timer()
    : _running(false), _elapsedTime(std::chrono::duration<double>::zero()) {}

void Timer::start() {
    if (!_running) {
        _elapsedTime = std::chrono::duration<double>::zero();
        _startTime = std::chrono::high_resolution_clock::now();
        _running = true;
    }
}

void Timer::stop() {
    if (_running) {
        auto now = std::chrono::high_resolution_clock::now();
        _elapsedTime += now - _startTime;
        _running = false;
    }
}

double Timer::elapsed() {
    if (!_running) {
        return _elapsedTime.count();
    }
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - _startTime).count();
}

void Timer::reset() {
    _elapsedTime = std::chrono::duration<double>::zero();
    _startTime = std::chrono::high_resolution_clock::now();
    _running = false;
}

void Timer::restart() {
    _elapsedTime = std::chrono::duration<double>::zero();
    _startTime = std::chrono::high_resolution_clock::now();
    _running = true;
}

bool Timer::isRunning() const {
    return _running;
}
