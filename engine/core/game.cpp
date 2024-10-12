#include "engineCore.hpp"
#include "engineExceptions.hpp"
#include "engineComponents.hpp"

engine::Game::Game()
    : _loadedScene("none")
{
    this->_factory = 0;
}

void engine::Game::loadObject(const ObjectRef &objref)
{

}

engine::Object engine::Game::buildObjectRef(const engine::ObjectRef &obj)
{
    Object temp;
    ObjectRef objRegisterd = this->_objects[obj.getName()];
    ObjectRef newObj = ObjectRef();

    for (const auto &item : objRegisterd.getAllBuildComponent()) {
        newObj.addBuildComponent(item, objRegisterd.getBuildComponent(item));
    }

    for (const auto &item : obj.getAllBuildParameter()) {
        newObj.addBuildParameter(item, obj.getBuildParameter(item));
    }
    temp = Object(newObj);
    if (!this->_factory)
        throw (engine::nullPtrError("factory as not been setup for game"));
    temp.buildEntity(*this->_factory);
    return (temp);
}

void engine::Game::loadScene(const std::string &sceneName)
{
    Scene scene = this->_scenes[sceneName];

    for (const auto &obj : scene.getObjects()) {
        this->_loadedGameObjects.push_back(std::pair<std::string, Object>(obj.first, this->buildObjectRef(obj.second)));
    }

    for (const auto &obj : scene.getHuds()) {
        this->_loadedGameHuds.push_back(std::pair<std::string, Object>(obj.first, this->buildObjectRef(obj.second)));
    }
}

void engine::Game::unloadScene(void)
{
    this->_loadedGameObjects = std::vector<std::pair<std::string, Object>>();
    this->_loadedScene = "none";
}

const engine::Scene &engine::Game::getScene(const std::string &name) const
{
    return (this->_scenes.find(name)->second);
}

const engine::ObjectRef &engine::Game::getObject(const std::string &name) const
{
    return (this->_objects.find(name)->second);
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


void engine::Game::addFactory(EntityFactory *factory)
{
    this->_factory = factory;
}

const std::vector<std::pair<std::string, engine::Object>> &engine::Game::getLoadedObjects(void)
{
    return (this->_loadedGameObjects);
}

const std::vector<std::pair<std::string, engine::Object>> &engine::Game::getLoadedHuds(void)
{
    return (this->_loadedGameHuds);
}

engine::EntityFactory *engine::Game::getFactory()
{
    return (this->_factory);
}
