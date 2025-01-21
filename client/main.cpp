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
    client game = client("secondary-game/game.yml");

    try
    {
        game.mainloop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
