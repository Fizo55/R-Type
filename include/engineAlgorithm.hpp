/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** engineAlgorithm
*/

#ifndef ENGINE_ALGORITHM_HPP_
    #define ENGINE_ALGORITHM_HPP_

    #include "engineComponents.hpp"
    #include "Timer.hpp"

    enum class MoveDirection {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        NONE
    };

    enum class Speed {
        NO_SPEED = 0
    };

    namespace engine_algorithm {

        class Acceleration {
            public:
                Acceleration(float acceleration);
                ~Acceleration() = default;

                float getAcceleration() const;
                void setAcceleration(float acceleration);

                float update(float deltaTime, float currentSpeed, float maxSpeed);

            private:

                float _acceleration;
        };

    };

#endif /* ENGINE_ALGORITHM_HPP_ */
