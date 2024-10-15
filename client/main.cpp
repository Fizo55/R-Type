/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engineCore.hpp"
#include "client.hpp"
#include "engineComponents.hpp"

using namespace engine;

int main(int ac, char **av)
{
    client game = client();

    game.mainloop();
}
