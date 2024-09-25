/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AEntity
*/

#pragma once

#include "includes/Interfaces/IEntity.hpp"

class AEntity : IEntity {
    public:

        // Constructor & Destructor //

        /**
         * @brief Destroy the AEntity object.
         *
         */
        ~AEntity() = default;

        /**
         * @brief Construct a new AEntity object.
         *
         * @param pos - position of the entity.
         * @param speed - speed of the entity.
         */
        AEntity(const Position &pos, int speed);

        // Getters //

        /**
         * @brief Get the Position object.
         *
         * @return std::pair<double, double> - position of the entity.
         */
        Position getPosition();

        /**
         * @brief Get the Speed object.
         *
         * @return double - speed of the entity.
         */
        double getSpeed();

        // Setters //

        /**
         * @brief Set the Position object.
         *
         * @param pos - position to set.
         */
        void setPosition(Position pos);

        /**
         * @brief Set the Speed object.
         *
         * @param speed - speed to set.
         */
        void setSpeed(double speed);

    protected:
        Position _position;
        int _speed;
};
