#include "gameServer.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <sstream>

argument::argument(const std::string &name, ARGUMENT_TYPE type, VALUE_TYPE valueType, std::any value)
  : name(name), type(type), valueType(valueType), value(value)
{

}

argumentParser::argumentParser(std::size_t minArgs, std::size_t maxArgs)
  : _minArgs(minArgs), _maxArgs(maxArgs)
{

}

argumentTypeFactory::argumentTypeFactory()
{

}

void argumentTypeFactory::registerBuildRoute(argument::VALUE_TYPE type, const std::function<std::any (const std::string &)> &callback)
{
    this->_buildRoutes[type] = callback;
}

std::any argumentTypeFactory::build(argument::VALUE_TYPE type, const std::string &value) const
{
    if (this->_buildRoutes.find(type) != this->_buildRoutes.end())
        return (this->_buildRoutes.at(type)(value));
    return (std::any(value));
}

void argumentParser::parseArgs(int argc, char **argv)
{
    bool registered = false;

    for (int i = 1; i < argc; ++i) {
        for (const auto &item : this->_lookingFor) {
            std::string temp(argv[i]);

            if (item.type == argument::ARGUMENT_TYPE::HELP && item.name == temp) {
                this->_builts[item.name] = argument(item.name, item.type, item.valueType, item.value);
                return;
            }
            if (item.type == argument::ARGUMENT_TYPE::SINGLE && item.name == temp) {
                this->_builts[item.name] = argument(item.name, item.type, item.valueType, item.value);
                registered = true;
                continue;
            }
            if (item.type == argument::ARGUMENT_TYPE::VALUE && item.name == temp) {
                this->_builts[item.name] = argument(item.name, item.type, item.valueType, this->_factory.build(item.valueType, argv[++i]));
                registered = true;
                break;
            }
            if (item.type == argument::ARGUMENT_TYPE::EQUAL && temp.rfind((item.name + "="), 0) == 0) {
                this->_builts[item.name] = argument(item.name, item.type, item.valueType, this->_factory.build(item.valueType, temp.substr((item.name + "=").length(), temp.length() - temp.find(item.name))));
                registered = true;
                continue;
            }
        }
        if (!registered) {
            if (this->_args.size() >= this->_maxArgs)
                throw std::runtime_error("too many arguments");
            if (this->_argsType.size() <= this->_args.size())
                this->_args.push_back(this->_factory.build(argument::STRING, argv[i]));
            else
                this->_args.push_back(this->_factory.build(this->_argsType[this->_args.size()], argv[i]));
        }
        registered = false;
    }
    if (this->_args.size() < this->_minArgs)
        throw std::runtime_error("too few arguments");
}

void argumentParser::registerArg(const argument &arg)
{
    this->_lookingFor.push_back(arg);
}

void argumentParser::registerUnamedArg(argument::VALUE_TYPE argsType)
{
    this->_argsType.push_back(argsType);
}

void argumentParser::attachFactory(const argumentTypeFactory &factory)
{
    this->_factory = factory;
}

const std::vector<std::any> &argumentParser::getUnnamedArgs(void) const
{
    return (this->_args);
}

const argument &argumentParser::getArg(const std::string &name) const
{
    return (this->_builts.at(name));
}

bool argumentParser::hasArg(const std::string &name) const
{
    return (this->_builts.find(name) != this->_builts.end());
}


std::any argument::base_string_builder(const std::string &str)
{
    return (std::any(str));
}

std::any argument::base_char_builder(const std::string &str)
{
    std::stringstream stringStream;
    char number;

    stringStream << str;
    stringStream >> number;

    return (std::any(number));
}

std::any argument::base_short_builder(const std::string &str)
{
    std::stringstream stringStream;
    short number;

    stringStream << str;
    stringStream >> number;

    return (std::any(number));
}

std::any argument::base_int_builder(const std::string &str)
{
    std::stringstream stringStream;
    int number;

    stringStream << str;
    stringStream >> number;

    return (std::any(number));
}

std::any argument::base_long_builder(const std::string &str)
{
    std::stringstream stringStream;
    long number;

    stringStream << str;
    stringStream >> number;

    return (std::any(number));
}

std::any argument::base_float_builder(const std::string &str)
{
    std::stringstream stringStream;
    double number;

    stringStream << str;
    stringStream >> number;

    return (std::any(number));
}
