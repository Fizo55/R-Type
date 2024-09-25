/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AEntity class implementation
*/

#include "AEntity.hpp"

AEntity::AEntity(const Position &pos, int speed)
{
    _position = pos;
    _speed = speed;
}

Position AEntity::getPosition()
{
    return _position;
}

double AEntity::getSpeed()
{
    return _speed;
}

void AEntity::setPosition(Position pos)
{
    _position = pos;
}

void AEntity::setSpeed(double speed)
{
    _speed = speed;
}
