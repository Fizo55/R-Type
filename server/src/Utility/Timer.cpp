/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Timer
*/

#include "Utility/Timer.hpp"

Timer::Timer()
    : _running(false), _elapsedTime(std::chrono::duration<double>::zero()) {}

void Timer::start() {
    if (!_running) {
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
        return _elapsedTime.count(); // Retourne simplement le temps écoulé si le timer n'est pas en cours
    }
    auto now = std::chrono::high_resolution_clock::now();
    return (_elapsedTime + (now - _startTime)).count();
}

void Timer::reset() {
    _elapsedTime = std::chrono::duration<double>::zero();
    _startTime = std::chrono::high_resolution_clock::now(); // Remet à zéro le point de départ
    _running = false; // Arrête le timer lors de la réinitialisation
}

void Timer::restart() {
    _elapsedTime = std::chrono::duration<double>::zero();
    _startTime = std::chrono::high_resolution_clock::now();
    _running = true; // Redémarre le timer lors de la réinitialisation
}

bool Timer::isRunning() const {
    return _running;
}
