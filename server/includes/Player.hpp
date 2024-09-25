/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
*/

#pragma once

#include <utility>

#include "includes/Abstracts/AEntity.hpp"

class Player : public AEntity {

    public:

        /**
         * @brief Destroy the Player object.
         *
         */
        ~Player() = default;

    private:
        Position _position;
        int _speed;

};
