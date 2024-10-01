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

void engine::Game::registerScene(const std::string &path)
{
    Scene scene = Scene(path);

    this->_scenes[scene.getName()] = scene;
}

void engine::Game::registerScene(const engine::Scene &scene)
{
    this->_scenes[scene.getName()] = scene;
}

void engine::Game::registerObject(const std::string &path)
{
    ObjectRef object = ObjectRef(path);

    this->_objects[object.getName()] = object;
}

void engine::Game::registerObject(const engine::ObjectRef &object)
{
    this->_objects[object.getName()] = object;
}

void engine::Game::unregisterScene(const std::string &name)
{
    this->_scenes.erase(name);
}

void engine::Game::unregisterObject(const std::string &name)
{
    this->_objects.erase(name);
}
