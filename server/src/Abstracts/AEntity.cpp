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

void AEntity::move(MoveDirection direction)
{
    switch (direction) {
    case MoveDirection::UP:
        _position.setY(_position.getY() - _speed);
        break;
    case MoveDirection::DOWN:
        _position.setY(_position.getY() + _speed);
        break;
    case MoveDirection::LEFT:
        _position.setX(_position.getX() - _speed);
        break;
    case MoveDirection::RIGHT:
        _position.setX(_position.getX() + _speed);
        break;
    }
}
