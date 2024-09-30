/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Acceleration
*/

#include "Acceleration.hpp"

Acceleration::Acceleration(float maxSpeed, float acceleration)
            : _currentSpeed(0), _maxSpeed(maxSpeed), _acceleration(acceleration) {}

float Acceleration::getCurrentSpeed() {
    return _currentSpeed;
}

float Acceleration::getMaxSpeed() {
    return _maxSpeed;
}

float Acceleration::getAcceleration() {
    return _acceleration;
}

void Acceleration::setCurrentSpeed(float speed) {
    this->_currentSpeed = speed;
}

void Acceleration::setMaxSpeed(float speed) {
    this->_maxSpeed = speed;
}

void Acceleration::setAcceleration(float acceleration) {
    this->_acceleration = acceleration;
}

void Acceleration::update(float deltaTime) {
    if (_currentSpeed < _maxSpeed) {
        _currentSpeed += _acceleration * deltaTime;
        if (_currentSpeed > _maxSpeed) {
            _currentSpeed = _maxSpeed;
        }
    }
}
