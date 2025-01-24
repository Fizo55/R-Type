#include "engineCore.hpp"

#include <memory>
#include <yaml-cpp/yaml.h>

engine::ObjectRef::ObjectRef()
    : _name("new object")
{

}

#include <sstream>
#include <cstring>

namespace engine {

engine::Object *engine::Object::deserializeFromBytes(const std::vector<char>& buffer) {
    size_t offset = 0;

    size_t nameLength = static_cast<unsigned char>(buffer[offset++]);
    std::string name(buffer.begin() + offset, buffer.begin() + offset + nameLength);
    offset += nameLength;

    engine::Object* obj = new engine::Object();
    obj->setName(name);

    size_t numComponents = static_cast<unsigned char>(buffer[offset++]);
    for (size_t i = 0; i < numComponents; ++i) {
        size_t compNameLength = static_cast<unsigned char>(buffer[offset++]);
        std::string compName(buffer.begin() + offset, buffer.begin() + offset + compNameLength);
        offset += compNameLength;

        size_t compValueLength = static_cast<unsigned char>(buffer[offset++]);
        std::string compValue(buffer.begin() + offset, buffer.begin() + offset + compValueLength);
        offset += compValueLength;

        obj->addBuildComponent(compName, compValue);
    }

    size_t numParameters = static_cast<unsigned char>(buffer[offset++]);
    for (size_t i = 0; i < numParameters; ++i) {
        size_t paramNameLength = static_cast<unsigned char>(buffer[offset++]);
        std::string paramName(buffer.begin() + offset, buffer.begin() + offset + paramNameLength);
        offset += paramNameLength;

        size_t numValues = static_cast<unsigned char>(buffer[offset++]);
        std::vector<std::any> paramValues;
        for (size_t j = 0; j < numValues; ++j) {
            uint8_t valueType = static_cast<unsigned char>(buffer[offset++]);
            switch (valueType) {
                case 0x01: {
                    size_t strLength = static_cast<unsigned char>(buffer[offset++]);
                    std::string strValue(buffer.begin() + offset, buffer.begin() + offset + strLength);
                    offset += strLength;
                    paramValues.push_back(strValue);
                    break;
                }
                case 0x02: {
                    int64_t intValue;
                    std::memcpy(&intValue, buffer.data() + offset, sizeof(int64_t));
                    offset += sizeof(int64_t);
                    paramValues.push_back(intValue);
                    break;
                }
                case 0x03: {
                    bool boolValue = static_cast<bool>(buffer[offset++]);
                    paramValues.push_back(boolValue);
                    break;
                }
                default:
                    throw std::runtime_error("Unknown parameter value type");
            }
        }
        obj->addBuildParameter(paramName, paramValues);
    }

    return obj;
}

std::vector<char> Object::serializeToBytes() const {
    std::vector<char> buffer;

    std::string name = getName();
    buffer.push_back(static_cast<char>(name.size()));
    buffer.insert(buffer.end(), name.begin(), name.end());

    const auto& components = getAllBuildComponent();
    buffer.push_back(static_cast<char>(components.size()));
    for (const auto& compName : components) {
        std::string compValue = getBuildComponent(compName);
        buffer.push_back(static_cast<char>(compName.size()));
        buffer.insert(buffer.end(), compName.begin(), compName.end());
        buffer.push_back(static_cast<char>(compValue.size()));
        buffer.insert(buffer.end(), compValue.begin(), compValue.end());
    }

    const auto& parameters = getAllBuildParameter();
    buffer.push_back(static_cast<char>(parameters.size()));
    for (const auto& paramName : parameters) {
        buffer.push_back(static_cast<char>(paramName.size()));
        buffer.insert(buffer.end(), paramName.begin(), paramName.end());

        const auto& paramValues = getBuildParameter(paramName);
        buffer.push_back(static_cast<char>(paramValues.size()));
        for (const auto& value : paramValues) {
            if (value.type() == typeid(std::string)) {
                std::string strValue = std::any_cast<std::string>(value);
                buffer.push_back(0x01);
                buffer.push_back(static_cast<char>(strValue.size()));
                buffer.insert(buffer.end(), strValue.begin(), strValue.end());
            } else if (value.type() == typeid(int64_t)) {
                int64_t intValue = std::any_cast<int64_t>(value);
                buffer.push_back(0x02);
                char intBuffer[sizeof(int64_t)];
                std::memcpy(intBuffer, &intValue, sizeof(int64_t));
                buffer.insert(buffer.end(), intBuffer, intBuffer + sizeof(int64_t));
            } else if (value.type() == typeid(bool)) {
                bool boolValue = std::any_cast<bool>(value);
                buffer.push_back(0x03);
                buffer.push_back(static_cast<char>(boolValue));
            }
        }
    }

    return buffer;
}

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