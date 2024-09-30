/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Movement
*/

#pragma once

#include "Utility/Timer.hpp"
#include "Utility/Position.hpp"
#include "Utility/Direction.hpp"
#include "Utility/Acceleration.hpp"

class Movement {

    public:

        // Constructor & Destructor //

        /**
         * @brief Construct a new Movement object.
         *
         * @param acceleration - acceleration of the entity.
         */
        Movement(Acceleration& acceleration, Position& pos);

        /**
         * @brief Destroy the Movement object.
         *
         */
        ~Movement() = default;

        // Getters //

        /**
         * @brief Get the Position of the entity.
         *
         * @return float - position of the entity.
         */
        Position& getPosition() const;

        /**
         * @brief Get the Acceleration of the entity.
         *
         * @return Acceleration& - acceleration of the entity.
         */
        Acceleration& getAcceleration() const;

        // Setters //

        /**
         * @brief Set the Position of the entity.
         *
         * @param pos - position to set.
         */
        void setPosition(Position& pos);

        /**
         * @brief Set the Acceleration of the entity.
         *
         * @param acceleration - acceleration to set.
         */
        void setAcceleration(Acceleration& acceleration);

        // Methods //

        /**
         * @brief Update the position of the entity.
         *
         * @param deltaTime - time between two frames.
         */
        void update(MoveDirection direction);

        // Operators //

        Movement& operator=(const Movement &other);

    private:

        Position &_position;
        Acceleration& _acceleration;
        Timer _timer;
};
