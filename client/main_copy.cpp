/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engineCore.hpp"
#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineScripting.hpp"
#include <cstdlib>

using namespace engine;

int main(int ac, char **av)
{
    client game = client("r-type/game.yml");

    game.mainloop();
    return 0;
}
