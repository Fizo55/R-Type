#include "engineComponents.hpp"
#include "engineExceptions.hpp"

engine_components::Volatile::Volatile(const std::string &name)
{
    this->name = name;
}

engine_components::Volatile::~Volatile()
{

}

void engine_components::Volatile::buildComponent(const engine::Entity &entity, const std::string &name, const std::vector<std::any> &args, engine::Registry &registry)
{
    registry.add_component(entity, Volatile{name});
}
