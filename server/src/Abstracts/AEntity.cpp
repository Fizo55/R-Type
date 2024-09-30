/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AEntity class implementation
*/

#include "Abstracts/AEntity.hpp"

AEntity::AEntity(Position &pos, float maxSpeed, float acceleration)
    : _position(pos), _acceleration(maxSpeed, acceleration), _movement(_acceleration, _position)
{
}

Position AEntity::getPosition()
{
    return _position;
}

float AEntity::getMaxSpeed()
{
    return _acceleration.getMaxSpeed();
}

float AEntity::getAcceleration()
{
    return _acceleration.getAcceleration();
}

void AEntity::setPosition(Position pos)
{
    _position = pos;
}

void AEntity::setMaxSpeed(float speed)
{
    _acceleration.setMaxSpeed(speed);
}

void AEntity::setAcceleration(float acceleration)
{
    _acceleration.setAcceleration(acceleration);
}

void AEntity::move(MoveDirection direction)
{
    _movement.update(direction);
}
