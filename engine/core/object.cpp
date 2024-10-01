#include "engineCore.hpp"

#include <yaml-cpp/yaml.h>

engine::ObjectRef::ObjectRef()
    : _name("new object")
{

}

engine::ObjectRef::ObjectRef(const std::string &path)
{
    YAML::Node obj = YAML::LoadFile(path);

    this->_name = obj["name"] ? obj["name"].as<std::string>() : "undefined";

    if (obj["components"]) {
        for (auto item : obj["components"]) {
            this->addBuildComponent(item["name"].as<std::string>(), item["type"].as<std::string>());
        }
    }
}

std::any engine::ObjectRef::parameterBuilder(const YAML::Node &node)
{
    if (bool bool_value; YAML::convert<bool>::decode(node, bool_value)) {
        return (node.as<bool>());
    }
    if (int64_t int_value; YAML::convert<int64_t>::decode(node, int_value)) {
        return (node.as<int64_t>());
    }
    if (double double_value; YAML::convert<double>::decode(node, double_value)) {
        return (node.as<double>());
    }
    return (node.as<std::string>());
}

const std::string &engine::ObjectRef::getName(void) const
{
    return (this->_name);
}

void engine::ObjectRef::setName(const std::string &name)
{
    this->_name = name;
}

void engine::ObjectRef::addBuildComponent(const std::string &name, const std::string &value)
{
    this->_buildComponents[name] = value;
}

void engine::ObjectRef::removeBuildComponent(const std::string &name)
{
    this->_buildComponents.erase(name);
}

const std::string &engine::ObjectRef::getBuildComponent(const std::string &name) const
{
    return (this->_buildComponents.at(name));
}

void engine::ObjectRef::addBuildParameter(const std::string &name, const std::vector<std::any> &values)
{
    this->_buildParameters[name] = values;
}

void engine::ObjectRef::removeBuildParameter(const std::string &name)
{
    this->_buildParameters.erase(name);
}

const std::vector<std::any> &engine::ObjectRef::getBuildParameter(const std::string &name) const
{
    return (this->_buildParameters.at(name));
}

void engine::Object::buildEntity(void)
{

}

const std::unique_ptr<engine::Entity> &engine::Object::getEntity(void) const
{
    return (this->_entity);
}

engine::Object::Object()
    : ObjectRef() {

}

engine::Object::Object(const std::string &path)
    : ObjectRef(path) {

}
