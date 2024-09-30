/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player
*/

#include "Player.hpp"

Player::Player(Position &pos, float maxSpeed, float acceleration)
    : AEntity(pos, maxSpeed, acceleration) {}