/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** IEntity
*/

#pragma once

#include <utility>

#include "Position.hpp"

class IEntity {
    public:

        /**
         * @brief Destroy the IEntity object.
         *
         */
        ~IEntity() = default;

        // Getters //

        /**
         * @brief Get the Position object.
         *
         * @return std::pair<double, double> - position of the entity.
         */
        virtual Position getPosition() = 0;

        /**
         * @brief Get the Speed object.
         *
         * @return double - speed of the entity.
         */
        virtual double getSpeed() = 0;


        // Setters //

        /**
         * @brief Set the Position object.
         *
         * @param pos - position to set.
         */
        virtual void setPosition(Position pos) = 0;

        /**
         * @brief Set the Speed object.
         *
         * @param speed - speed to set.
         */
        virtual void setSpeed(double speed) = 0;

};
