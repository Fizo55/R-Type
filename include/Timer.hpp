/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Timer
*/

#pragma once

#include <chrono>

class Timer {

    public:

        /**
         * @brief Construct a new Timer object.
         *
         */
        Timer();

        /**
         * @brief Destroy the Timer object.
         *
         */
        ~Timer() = default;

        /**
         * @brief Start the timer.
         *
         */
        void start();

        /**
         * @brief Stop the timer.
         *
         */
        void stop();

        /**
         * @brief Get the elapsed time.
         *
         * @return double - elapsed time.
         */
        double elapsed();

        /**
         * @brief Reset the timer.
         *
         */
        void reset();

        /**
         * @brief Restart the timer.
         *
         */
        void restart();

        /**
         * @brief Check if the timer is running.
         *
         * @return true - if the timer is running.
         * @return false - if the timer is not running.
         */
        bool isRunning() const;

    private:

        std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
        std::chrono::duration<double> _elapsedTime;
        bool _running;

};
