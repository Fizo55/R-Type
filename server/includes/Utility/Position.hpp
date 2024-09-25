/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Position
*/

#pragma once

#include <utility>

class Position {

    public:

        // Constructor //

        /**
         * @brief Construct a new Position object.
         *
         */
        Position() = default;

        /**
         * @brief Construct a new Position object.
         *
         * @param x x position.
         * @param y y position.
         */
        Position(double x, double y);

        /**
         * @brief Construct a copy of Position object.
         *
         * @param pos position to copy.
         */
        Position(Position &pos) = default;

        /**
         * @brief Destroy the Position object.
         *
         */
        ~Position() = default;

        // Getters //

        /**
         * @brief Get the X axis.
         *
         * @return double - x position.
         */
        double getX();

        /**
         * @brief Get the Y axis.
         *
         * @return double - y position.
         */
        double getY();

        /**
         * @brief Get the Position object.
         *
         * @return std::pair<double, double> - position.
         */
        std::pair<double, double> getPosition();

        // Setters //

        /**
         * @brief Set the X axis.
         *
         * @param x x position.
         */
        void setX(double x);

        /**
         * @brief Set the Y axis.
         *
         * @param y y position.
         */
        void setY(double y);

        /**
         * @brief Set the Position object.
         *
         * @param pos position to set.
         */
        void setPosition(std::pair<double, double> pos);

        // Operator //

        /**
         * @brief Copy the Position object.
         *
         * @param pos position to copy.
         * @return Position& - copied position.
         */
        Position &operator=(const Position &pos);

    private:
        double _x;
        double _y;
};

