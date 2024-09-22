/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "common.hpp"
#include "libs.hpp"
#include <vector>
#include <memory>

int main(int ac, char **av)
{
    std::vector<std::unique_ptr<ILib>> libs = getLibs(getType(), ac, av);
}