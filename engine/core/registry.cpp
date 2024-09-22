#include "engineCore.hpp"

engine::Entity engine::Registry::create_entity() {
    return Entity(_next_entity_id++);
}

void engine::Registry::destroy_entity(Entity entity) {
    auto entity_id = static_cast<uint64_t>(entity);
    for (auto& [type, pool] : _component_pools) {
        pool->remove_component(entity_id);
    }
}
