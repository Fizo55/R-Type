/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** libs
*/

#ifndef LIBS_HPP_
#define LIBS_HPP_

#include <iostream>
#include <vector>
#include <memory>

    std::vector<std::unique_ptr<ILib>> getLibs(std::string type, int ac, char **av);

class ILib {
    public:
        virtual ~ILib() = default;
};

class GraphLib : public ILib {
    public:
        virtual ~GraphLib() = default;
};

class AudioLib : public ILib {
    public:
        virtual ~AudioLib() = default;
};

class NetworkLib : public ILib {
    public:
        virtual ~NetworkLib() = default;
};

class CoreLib : public ILib {
    public:
        virtual ~CoreLib() = default;
};

class AlgoLib : public ILib {
    public:
        virtual ~AlgoLib() = default;
};

#endif /* !LIBS_HPP_ */
