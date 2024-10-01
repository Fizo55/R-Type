#include "engineCore.hpp"
#include <yaml-cpp/yaml.h>

engine::Scene::Scene()
    : _name("new scene")
{
}

engine::Scene::Scene(const std::string &sceneConfig)
{
    YAML::Node scene = YAML::LoadFile(sceneConfig);

    this->_name = scene["name"] ? scene["name"].as<std::string>() : "undefined";

    for (auto item : scene["objects"])
        std::cout << item["name"] << std::endl;
}

const std::string &engine::Scene::getName(void) const
{
    return (this->_name);
}

void engine::Scene::setName(const std::string &)
{

}

void engine::Scene::addHud(const std::string &, const std::string &)
{

}

void engine::Scene::addObject(const std::string &, const std::string &)
{

}

void engine::Scene::removeHud(const std::string &)
{

}

void engine::Scene::removeObject(const std::string &)
{

}

const std::vector<std::pair<std::string, std::string>> &engine::Scene::getHuds(void) const
{
    return (_huds);
}

const std::vector<std::pair<std::string, std::string>> &engine::Scene::getObjects(void) const
{
    return (_objects);
}
