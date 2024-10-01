#include "engineCore.hpp"

engine::Game::Game()
{

}

const engine::Scene &engine::Game::getScene(const std::string &name) const
{
    return (this->_scenes.at(name));
}

const engine::ObjectRef &engine::Game::getObject(const std::string &name) const
{
    return (this->_objects.at(name));
}

void engine::Game::registerScene(const std::string &)
{

}

void engine::Game::registerScene(const engine::Scene &)
{

}

void engine::Game::registerObject(const std::string &)
{

}

void engine::Game::registerObject(const engine::ObjectRef &)
{

}

void engine::Game::unregisterScene(const std::string &)
{

}

void engine::Game::unregisterObject(const std::string &)
{

}
