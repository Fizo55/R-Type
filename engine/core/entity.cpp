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