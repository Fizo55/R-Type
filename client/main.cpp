/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engineCore.hpp"

using namespace engine;

int main(int ac, char **av)
{
    Game theGame = Game();

    theGame.registerObject("assets/objects/test.yml");

    std::cout << theGame.getObject("obj0") << std::endl;
    theGame.registerScene("assets/scenes/main_scene.yml");

    std::cout << theGame.getScene("scene0").getObjects().front().second << std::endl;

    theGame.loadScene("scene0");
}