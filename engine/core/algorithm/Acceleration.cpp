/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Acceleration
*/

#include "engineAlgorithm.hpp"

engine_algorithm::Acceleration::Acceleration(float acceleration) : _acceleration(acceleration) {}

float engine_algorithm::Acceleration::getAcceleration() const {
    return _acceleration;
}

void engine_algorithm::Acceleration::setAcceleration(float acceleration) {
    _acceleration = acceleration;
}

float engine_algorithm::Acceleration::update(float deltaTime, float currentSpeed, float maxSpeed) {
    if (currentSpeed < maxSpeed) {
        currentSpeed += _acceleration * deltaTime;
        if (currentSpeed > maxSpeed) {
            currentSpeed = maxSpeed;
        }
    }
    return currentSpeed;
}
