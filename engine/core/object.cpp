#include "engineCore.hpp"

#include <memory>
#include <yaml-cpp/yaml.h>

engine::ObjectRef::ObjectRef()
    : _name("new object")
{

}

namespace engine {

engine::ObjectRef::ObjectRef(const std::string &path)
{
    YAML::Node obj = YAML::LoadFile(path);

    this->_name = obj["name"] ? obj["name"].as<std::string>() : "undefined";

    if (obj["components"]) {
        for (const auto &item : obj["components"]) {
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

const std::vector<std::any> engine::ObjectRef::getBuildParameter(const std::string &name) const
{
    if (this->_buildParameters.find(name) == this->_buildParameters.end())
        return (std::vector<std::any>());
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

const std::unique_ptr<engine::Entity> &engine::Object::getEntity(void) const
{
    return (this->_entity);
}

engine::Object::Object()
    : ObjectRef() {

}

engine::Object::Object(const ObjectRef &other) {
    this->setName(other.getName());

    for (const auto &item : other.getAllBuildComponent())
        this->addBuildComponent(item, other.getBuildComponent(item));
    for (const auto &item : other.getAllBuildParameter())
        this->addBuildParameter(item, other.getBuildParameter(item));
}

void engine::Object::buildEntity(EntityFactory *factory) {
    std::vector<EntityBuildData> buildData;

    for (const auto &item : this->getAllBuildComponent()) {
        buildData.push_back(EntityBuildData(this->getBuildComponent(item), item, this->getBuildParameter(item)));
    }

    this->_entity = std::make_unique<Entity>(factory->createEntityComponentReady(buildData));
}

nlohmann::json Object::anyToJson(const std::any &value)
{
    nlohmann::json j;
    if (value.type() == typeid(std::string)) {
        j["type"] = "string";
        j["value"] = std::any_cast<std::string>(value);
    }
    else if (value.type() == typeid(int64_t)) {
        j["type"] = "int64";
        j["value"] = std::any_cast<int64_t>(value);
    }
    else if (value.type() == typeid(bool)) {
        j["type"] = "bool";
        j["value"] = std::any_cast<bool>(value);
    }
    else {
        throw std::runtime_error("Unsupported std::any type during serialization");
    }
    return j;
}

std::any Object::jsonToAny(const nlohmann::json &j)
{
    std::string type = j.at("type").get<std::string>();
    if (type == "string") {
        return j.at("value").get<std::string>();
    }
    else if (type == "int64") {
        return j.at("value").get<int64_t>();
    }
    else if (type == "bool") {
        return j.at("value").get<bool>();
    }
    else {
        throw std::runtime_error("Unsupported type during deserialization: " + type);
    }
}

std::string Object::serializeToJson() const
{
    nlohmann::json j;
    j["name"] = this->getName();

    j["buildComponents"] = this->getAllBuildComponent();
    for (const auto &comp : this->getAllBuildComponent()) {
        j["buildComponents"][comp] = this->getBuildComponent(comp);
    }

    j["buildParameters"] = nlohmann::json::array();
    for (const auto &paramName : this->getAllBuildParameter()) {
        for (const auto &value : this->getBuildParameter(paramName)) {
            j["buildParameters"].push_back(anyToJson(value));
        }
    }

    return j.dump();
}

std::unique_ptr<Object> Object::deserializeFromJson(const std::string &jsonString)
{
    nlohmann::json j = nlohmann::json::parse(jsonString);
    auto obj = std::make_unique<Object>();

    if (j.contains("name") && j["name"].is_string()) {
        obj->setName(j["name"].get<std::string>());
    }
    else {
        throw std::runtime_error("Invalid or missing 'name' field in JSON");
    }

    if (j.contains("buildComponents") && j["buildComponents"].is_object()) {
        for (auto &el : j["buildComponents"].items()) {
            obj->addBuildComponent(el.key(), el.value().get<std::string>());
        }
    }
    else {
        throw std::runtime_error("Invalid or missing 'buildComponents' field in JSON");
    }

    if (j.contains("buildParameters") && j["buildParameters"].is_object()) {
        for (const auto& [paramName, paramValues] : j["buildParameters"].items()) {
            if (!paramValues.is_array()) {
                throw std::runtime_error("Each 'buildParameters' entry must be an array.");
            }
            std::vector<std::any> values;
            for (const auto& item : paramValues) {
                std::any value = jsonToAny(item);
                values.emplace_back(value);
            }
            obj->addBuildParameter(paramName, values);
        }
    }
    else {
        throw std::runtime_error("Invalid or missing 'buildParameters' field in JSON");
    }

    return obj;
}
}

std::ostream &operator<<(std::ostream &os, engine::ObjectRef const &obj) {
    std::size_t i = 0;

    os << "<ObjectRef[" << obj.getName() << "]";

    if (!obj.getAllBuildComponent().empty()) {
        os << " components={";
        for (const auto &item : obj.getAllBuildComponent()) {
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
        for (const auto &item : obj.getAllBuildParameter()) {
            std::size_t j = 0;

            if (i)
                os << ", ";
            os << item << "=(";
            for (const auto &param : obj.getBuildParameter(item)) {
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
        for (const auto &item : obj.getAllBuildComponent()) {
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
        for (const auto &item : obj.getAllBuildParameter()) {
            std::size_t j = 0;

            if (i)
                os << ", ";
            os << item << "=(";
            for (const auto &param : obj.getBuildParameter(item)) {
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
