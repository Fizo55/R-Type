/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
*/

#pragma once

#include <utility>

#include "Abstracts/AEntity.hpp"

class Player : public AEntity {

    public:

        // Constructor & Destructor //

        /**
         * @brief Construct a new Player object.
         *
         * @param pos - position of the player.
         * @param maxSpeed - max speed of the player.
         * @param acceleration - acceleration of the player.
         */
        Player(Position &pos, float maxSpeed, float acceleration);

        /**
         * @brief Destroy the Player object.
         *
         */
        ~Player() = default;

    private:

};
