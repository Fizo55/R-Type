/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Movement
*/

#include "engineAlgorithm.hpp"

engine_algorithm::Movement::Movement()
{
}


void engine_algorithm::Movement::update(MoveDirection direction, engine_components::Position &position, Acceleration &acceleration, float currentSpeed, float maxSpeed) {

    if (direction == MoveDirection::NONE) {
        _timer.stop();
        return;
    }

    _timer.start();
    double deltaTime = _timer.elapsed();
    currentSpeed = acceleration.update(deltaTime, currentSpeed, maxSpeed);

    switch (direction) {
        case MoveDirection::UP:
            position.coordinates.y = position.coordinates.y - currentSpeed * deltaTime;
            break;
        case MoveDirection::DOWN:
            position.coordinates.y = position.coordinates.y + currentSpeed * deltaTime;
            break;
        case MoveDirection::LEFT:
            position.coordinates.x = position.coordinates.x - currentSpeed * deltaTime;
            break;
        case MoveDirection::RIGHT:
            position.coordinates.x = position.coordinates.x + currentSpeed * deltaTime;
            break;
    }
}
