/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AEntity class implementation
*/

#include "AEntity.hpp"

AEntity::AEntity(Position &pos, int maxSpeed, int acceleration)
    : _position(pos), _acceleration(maxSpeed, acceleration), _movement(_acceleration, _position)
{
}

Position AEntity::getPosition()
{
    return _position;
}

double AEntity::getMaxSpeed()
{
    return _acceleration.getMaxSpeed();
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
