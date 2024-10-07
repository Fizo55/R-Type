#include "engineCore.hpp"

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

engine::EntityBuildData::EntityBuildData(const std::string &nameRef, const std::vector<std::any> &args)
  :  nameRef(nameRef), buildArgs(args) {

}

engine::ComponentBuildRoute::ComponentBuildRoute(const std::string &name, const std::function<void(const Entity &, const std::vector<std::any> &)> &callback)
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

void engine::EntityFactory::registerBuildComponentRoute(const ComponentBuildRoute &route)
{
    this->_routes[route.name] = route;
}

void engine::EntityFactory::buildComponent(const Entity &entity, const EntityBuildData &data)
{

}

engine::Entity engine::EntityFactory::createEntityComponentReady(const std::vector<EntityBuildData> &data)
{
    Entity entity = this->_registry.create_entity();

    for (auto buildData : data) {
        this->buildComponent(entity, buildData);
    }
    return (entity);
}
