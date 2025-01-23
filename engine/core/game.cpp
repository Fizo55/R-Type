#include "engineCore.hpp"
#include "engineExceptions.hpp"
#include "engineComponents.hpp"

engine::Game::Game()
    : _loadedScene("none")
{
    this->_factory = 0;
}

engine::Object *engine::Game::buildObjectRef(const engine::ObjectRef &obj, const std::string &name)
{
    Object *temp;
    ObjectRef objRegisterd = this->_objects[obj.getName()];
    ObjectRef newObj = ObjectRef();

    for (const auto &item : objRegisterd.getAllBuildComponent()) {
        newObj.addBuildComponent(item, objRegisterd.getBuildComponent(item));
    }

    for (const auto &item : obj.getAllBuildParameter()) {
        newObj.addBuildParameter(item, obj.getBuildParameter(item));
    }
    temp = new Object(newObj);
    if (!this->_factory)
        throw (engine::nullPtrError("factory as not been setup for game"));
    temp->setName(name);
    temp->buildEntity(this->_factory);
    return (temp);
}

engine::Game::~Game() {
    for (auto temp : this->_loadedGameObjects)
        delete temp;

    for (auto temp : this->_loadedGameHuds)
        delete temp;

    this->_loadedScene = "none";

    this->_loadedGameObjects.clear();
    this->_loadedGameHuds.clear();
}

void engine::Game::loadScene(const std::string &sceneName)
{
    Scene scene = this->_scenes[sceneName];

    for (auto temp : this->_loadedGameObjects)
        delete temp;

    for (auto temp : this->_loadedGameHuds)
        delete temp;

    this->_loadedGameObjects.clear();
    this->_loadedGameHuds.clear();

    for (const auto &obj : scene.getObjects()) {
        this->_loadedGameObjects.push_back(this->buildObjectRef(obj.second, obj.first));
    }

    for (const auto &obj : scene.getHuds()) {
        this->_loadedGameHuds.push_back(this->buildObjectRef(obj.second, obj.first));
    }
}

void engine::Game::loadObject(engine::Object *obj)
{
    this->_loadedGameObjects.push_back(obj);
}

void engine::Game::unloadObject(engine::Object *obj)
{
    this->_loadedGameObjects.erase(std::remove_if(
        this->_loadedGameObjects.begin(), this->_loadedGameObjects.end(),
        [&](Object *&x) {
            return ((long)x == (long)obj);
        }), this->_loadedGameObjects.end()
    );
}

void engine::Game::unloadHud(engine::Object *obj)
{
    this->_loadedGameHuds.erase(std::remove_if(
        this->_loadedGameHuds.begin(), this->_loadedGameHuds.end(),
        [&](Object *&x) {
            return ((long)x == (long)obj);
        }), this->_loadedGameHuds.end()
    );
}

void engine::Game::loadHud(engine::Object *obj)
{
    this->_loadedGameHuds.push_back(obj);
}

void engine::Game::unloadScene(void)
{
    for (auto temp : this->_loadedGameObjects)
        delete temp;

    for (auto temp : this->_loadedGameHuds)
        delete temp;

    this->_loadedGameObjects.clear();
    this->_loadedGameHuds.clear();
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

const std::vector<engine::Object *> &engine::Game::getLoadedObjects(void)
{
    return (this->_loadedGameObjects);
}

const std::vector<engine::Object *> &engine::Game::getLoadedHuds(void)
{
    return (this->_loadedGameHuds);
}

std::int64_t engine::Game::readDBInt(std::size_t pos)
{
    auto read = this->_db.readDB(pos);

    if (!read)
        return (0);
    return (std::any_cast<std::int64_t>(read));
}

void engine::Game::writeDBInt(std::size_t pos, std::int64_t value)
{
    this->_db.writeDB(pos, std::any(value));
}

engine::EntityFactory *engine::Game::getFactory(void)
{
    return (this->_factory);
}

engine::GameDB::GameDB()
{

}

engine::GameDB::~GameDB()
{
    this->_dbRegister.clear();
}

std::optional<std::any> engine::GameDB::readDB(std::size_t pos)
{
    if (this->_dbRegister.find(pos) != this->_dbRegister.end())
        return (this->_dbRegister.at(pos));
    return {};
}
void engine::GameDB::writeDB(std::size_t pos, const std::any &content)
{
    this->_dbRegister.emplace(pos, content);
}
