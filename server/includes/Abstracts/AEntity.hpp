/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AEntity
*/

#pragma once

#include "Interfaces/IEntity.hpp"
#include "Utility/Movement.hpp"
#include "Utility/Acceleration.hpp"

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
         * @param maxSpeed - max speed of the entity.
         * @param acceleration - acceleration of the entity.
         */
        AEntity(Position &pos, int maxSpeed, int acceleration);

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
         * @return double - max speed of the entity.
         */
        double getMaxSpeed();

        /**
         * @brief Get the Acceleration object.
         *
         * @return double - acceleration of the entity.
         */
        double getAcceleration();

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
        void setMaxSpeed(float speed);

        /**
         * @brief Set the Acceleration object.
         *
         * @param acceleration - acceleration to set.
         */
        void setAcceleration(float acceleration);

        // Methods //

        /**
         * @brief Move the entity.
         *
         * @param direction - direction to move.
         */
        void move(MoveDirection direction);

    protected:

        Position &_position;
        Movement _movement;
        Acceleration _acceleration;
};
