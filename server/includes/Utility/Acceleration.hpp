/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Acceleration
*/

#pragma once

class Acceleration {

    public:

        // Constructor & Destructor //

        /**
         * @brief Construct a new Acceleration object.
         *
         * @param maxSpeed - maximum speed of the entity.
         * @param acceleration - acceleration of the entity.
         */
        Acceleration(float maxSpeed, float acceleration);

        /**
         * @brief Destroy the Acceleration object
         *
         */
        ~Acceleration() = default;

        // Getters //

        /**
         * @brief Get the Current Speed object
         *
         * @return float - current speed of the entity.
         */
        float getCurrentSpeed();

        /**
         * @brief Get the Max Speed object.
         *
         * @return float - maximum speed of the entity.
         */
        float getMaxSpeed();

        /**
         * @brief Get the Acceleration object.
         *
         * @return float - acceleration of the entity.
         */
        float getAcceleration();

        // Setters //

        /**
         * @brief Set the Current Speed object.
         *
         * @param speed - speed to set.
         */
        void setCurrentSpeed(float speed);

        /**
         * @brief Set the Max Speed object.
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
         * @brief Update the entity's speed.
         *
         * @param deltaTime - time since last update.
         */
        void update(float deltaTime);

        // Operators //

        /**
         * @brief Overloaded operator=.
         *
         * @param other - acceleration to copy.
         * @return Acceleration& - copied acceleration.
         */
        Acceleration& operator=(const Acceleration &other);

    private:

        float _currentSpeed;
        float _maxSpeed;
        float _acceleration;

};