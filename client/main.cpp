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
    // client game = client("r-type/game.yml");

    // game.mainloop();
    // system("./r-type/scripts/test.lua");
    std::cout << "mstep1" << std::endl;

    engine::ScriptOrchestrator scriptOrchestrator;
    std::cout << "mstep2" << std::endl;
    scriptOrchestrator.registerScript("test_script", "./r-type/scripts/test.lua");
    std::cout << "mstep3" << std::endl;
    scriptOrchestrator.runLuaFunctions("test_script", "move_player", {});


    return 0;
}
