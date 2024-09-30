/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Position
*/

#include "Utility/Position.hpp"

Position::Position(double x, double y)
{
    _x = x;
    _y = y;
}

double Position::getX()
{
    return _x;
}

double Position::getY()
{
    return _y;
}

std::pair<double, double> Position::getPosition()
{
    return std::make_pair(_x, _y);
}

void Position::setX(double x)
{
    _x = x;
}

void Position::setY(double y)
{
    _y = y;
}

void Position::setPosition(std::pair<double, double> pos)
{
    _x = pos.first;
    _y = pos.second;
}

Position& Position::operator=(const Position &pos)
{
    _x = pos._x;
    _y = pos._y;
    return *this;
}
