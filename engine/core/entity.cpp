#include "engineCore.hpp"
#include "engineExceptions.hpp"

engine::Entity::Entity(std::size_t id) : _id(id) {}

engine::Entity::operator std::size_t() const {
    return _id;
}

void engine::Entity::attach_system(std::unique_ptr<engine::EcsSystem> system) {
    _systems.push_back(std::move(system));
}

void engine::Entity::update_systems(engine::Registry& registry, float delta_time) {
    for (auto& system : _systems) {
        system->update(registry, *this, delta_time);
    }
}

engine::EntityBuildData::EntityBuildData(const std::string &nameRef, const std::string &name, const std::vector<std::any> &args)
  :  nameRef(nameRef), name(name), buildArgs(args) {

}

engine::ComponentBuildRoute::ComponentBuildRoute(const std::string &name, const std::function<void(const Entity &, const std::string &, const std::vector<std::any> &, Registry &)> &callback)
  :  name(name), callback(callback)
{

}

engine::Entity engine::EntityFactory::createEntity(void)
{
    return (this->_registry.create_entity());
}

engine::EntityFactory::EntityFactory()
{

}

engine::Registry &engine::EntityFactory::getRegistry(void)
{
    return (this->_registry);
}

void engine::EntityFactory::registerBuildComponentRoute(const ComponentBuildRoute &route)
{
    this->_routes[route.name] = route;
}

void engine::EntityFactory::buildComponent(const Entity &entity, const EntityBuildData &data)
{
    if (this->_routes.find(data.nameRef) == this->_routes.end())
        throw engine::componentBuildError("component not found in registered components routes.");
    this->_routes[data.nameRef].callback(entity, data.name, data.buildArgs, this->_registry);
}

engine::Entity engine::EntityFactory::createEntityComponentReady(const std::vector<EntityBuildData> &data)
{
    Entity entity = this->_registry.create_entity();

    for (auto buildData : data) {
        this->buildComponent(entity, buildData);
    }
    return (entity);
}
