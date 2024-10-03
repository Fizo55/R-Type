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
    return (this->_buildComponents.find(name)->second);
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
    return (this->_buildParameters.find(name)->second);
}

std::vector<std::string> engine::ObjectRef::getAllBuildComponent(void) const
{
    std::vector<std::string> keys;

    for (auto it = this->_buildComponents.begin(); it != this->_buildComponents.end(); ++it) {
        keys.push_back(it->first);
    }

    return (keys);
}

std::vector<std::string> engine::ObjectRef::getAllBuildParameter(void) const
{
        std::vector<std::string> keys;

    for (auto it = this->_buildParameters.begin(); it != this->_buildParameters.end(); ++it) {
        keys.push_back(it->first);
    }

    return (keys);
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

engine::Object::Object(const ObjectRef &other) {
    this->setName(other.getName());

    for (auto item : other.getAllBuildComponent())
        this->addBuildComponent(item, other.getBuildComponent(item));
    for (auto item : other.getAllBuildParameter())
        this->addBuildParameter(item, other.getBuildParameter(item));

    this->buildEntity();
}

void engine::Object::buildEntity(void) {
    this->_entity = std::make_unique<Entity>(1);
}

std::ostream &operator<<(std::ostream &os, engine::ObjectRef const &obj) {
    std::size_t i = 0;

    os << "<ObjectRef[" << obj.getName() << "]";

    if (!obj.getAllBuildComponent().empty()) {
        os << " components={";
        for (auto item : obj.getAllBuildComponent()) {
            if (i)
                    os << ", ";
            os << item << "=" << obj.getBuildComponent(item);

            ++i;
        }
        os << "}";
    }
    i = 0;
    if (!obj.getAllBuildParameter().empty()) {
        os << " parameters={";
        for (auto item : obj.getAllBuildParameter()) {
            std::size_t j = 0;

            if (i)
                os << ", ";
            os << item << "=(";
            for (auto param : obj.getBuildParameter(item)) {
                if (j)
                    os << " ";
                if (param.type() == typeid(std::string))
                    os << std::any_cast<std::string>(param);
                else if (param.type() == typeid(int64_t))
                    os << std::any_cast<int64_t>(param);
                else
                    os << "unknown";

                ++j;
            }
            os << ")";

            ++i;
        }
        os << "}";
    }
    os << ">";
    return (os);
}

std::ostream &operator<<(std::ostream &os, engine::Object const &obj) {
    std::size_t i = 0;

    os << "<Object[" << obj.getName() << "]";

    if (!obj.getAllBuildComponent().empty()) {
        os << " components={";
        for (auto item : obj.getAllBuildComponent()) {
            if (i)
                    os << ", ";
            os << item << "=" << obj.getBuildComponent(item);

            ++i;
        }
        os << "}";
    }
    i = 0;
    if (!obj.getAllBuildParameter().empty()) {
        os << " parameters={";
        for (auto item : obj.getAllBuildParameter()) {
            std::size_t j = 0;

            if (i)
                os << ", ";
            os << item << "=(";
            for (auto param : obj.getBuildParameter(item)) {
                if (j)
                    os << " ";
                if (param.type() == typeid(std::string))
                    os << std::any_cast<std::string>(param);
                else if (param.type() == typeid(int64_t))
                    os << std::any_cast<int64_t>(param);
                else
                    os << "unknown";

                ++j;
            }
            os << ")";

            ++i;
        }
        os << "}";
    }
    os << ">";
    return (os);
}