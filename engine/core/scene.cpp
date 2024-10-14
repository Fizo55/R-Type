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

    if (scene["objects"]) {
        for (const auto &item : scene["objects"]) {
            ObjectRef temp = ObjectRef();

            temp.setName(item["id"].as<std::string>());
            for(YAML::const_iterator it = item["values"].begin(); it != item["values"].end(); ++it) {
                std::vector<std::any> params;

                for (const auto &param : it->second) {
                    params.push_back(ObjectRef::parameterBuilder(param));
                }
                temp.addBuildParameter(it->first.as<std::string>(), params);
            }
            this->addObject(item["name"].as<std::string>(), temp);
        }
    }

    if (scene["huds"]) {
        for (const auto &item : scene["huds"]) {
            ObjectRef temp = ObjectRef();

            temp.setName(item["id"].as<std::string>());
            for(YAML::const_iterator it = item["values"].begin(); it != item["values"].end(); ++it) {
                std::vector<std::any> params;

                for (const auto &param : it->second) {
                    params.push_back(ObjectRef::parameterBuilder(param));
                }
                temp.addBuildParameter(it->first.as<std::string>(), params);
            }
            this->addHud(item["name"].as<std::string>(), temp);
        }
    }
}

const std::string &engine::Scene::getName(void) const
{
    return (this->_name);
}

void engine::Scene::setName(const std::string &name)
{
    this->_name = name;
}

void engine::Scene::addHud(const std::string &name, const engine::ObjectRef &ref)
{
    this->_huds.push_back(std::pair<std::string, engine::ObjectRef>(name, ref));
}

void engine::Scene::addObject(const std::string &name, const engine::ObjectRef &ref)
{
    this->_objects.push_back(std::pair<std::string, engine::ObjectRef>(name, ref));
}

const std::vector<std::pair<std::string, engine::ObjectRef>> &engine::Scene::getHuds(void) const
{
    return (_huds);
}

const std::vector<std::pair<std::string, engine::ObjectRef>> &engine::Scene::getObjects(void) const
{
    return (_objects);
}
