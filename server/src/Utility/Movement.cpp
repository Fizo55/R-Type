/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Movement
*/

#include "Utility/Movement.hpp"

Movement::Movement(Acceleration& acceleration, Position& pos)
    : _acceleration(acceleration), _position(pos)
{
}

void Movement::update(MoveDirection direction) {

    if (direction == MoveDirection::NONE) {
        _timer.stop();
        _acceleration.setCurrentSpeed(Speed::NO_SPEED);
        return;
    }

    _timer.start();
    double deltaTime = _timer.elapsed();
    _acceleration.update(deltaTime);

    float currentSpeed = _acceleration.getCurrentSpeed();
    switch (direction) {
        case MoveDirection::UP:
            _position.setY(_position.getY() - currentSpeed * deltaTime);
            break;
        case MoveDirection::DOWN:
            _position.setY(_position.getY() + currentSpeed * deltaTime);
            break;
        case MoveDirection::LEFT:
            _position.setX(_position.getX() - currentSpeed * deltaTime);
            break;
        case MoveDirection::RIGHT:
            _position.setX(_position.getX() + currentSpeed * deltaTime);
            break;
    }
}

Position& Movement::getPosition() const {
    return _position;
}

Acceleration& Movement::getAcceleration() const {
    return _acceleration;
}

void Movement::setPosition(Position& pos) {
    _position = pos;
}

void Movement::setAcceleration(Acceleration& acceleration) {
    _acceleration = acceleration;
}

Movement& Movement::operator=(const Movement &other) {
    if (this != &other) {
        this->_acceleration = other._acceleration;
        this->_position = other._position;
    }
    return *this;
}
