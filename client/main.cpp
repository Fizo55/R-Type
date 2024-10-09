/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engineCore.hpp"
#include "engineComponents.hpp"

using namespace engine;

int main(int ac, char **av)
{
    Game theGame = Game();
    EntityFactory factory = EntityFactory();

    factory.registerComponent<engine_components::Position>();

    factory.registerBuildComponentRoute(ComponentBuildRoute("position", engine_components::Position::buildComponent));

    theGame.addFactory(&factory);

    theGame.registerObject("assets/objects/test.yml");
    theGame.registerScene("assets/scenes/main_scene.yml");

    theGame.loadScene("scene0");
}