//
// Created by erwan on 16/09/2024.
//

#ifndef ENGINE_CORE_TPP_
    #define ENGINE_CORE_TPP_

    #include <stdexcept>
    #include <typeindex>
    #include <unordered_map>
    #include <memory>
    #include <optional>
    #include <vector>
    #include <iostream>

    #include "engineCore.hpp"

    namespace engine {
        template <typename Component>
        ComponentPool<Component>& Registry::register_component() {
            auto type_idx = std::type_index(typeid(Component));
            if (_component_pools.find(type_idx) == _component_pools.end()) {
                _component_pools[type_idx] = std::make_unique<ComponentPool<Component>>();
            }
            return *static_cast<ComponentPool<Component>*>(_component_pools[type_idx].get());
        }

        template <typename Component>
        void Registry::add_component(const Entity& entity, const Component& component) {
            auto& pool = register_component<Component>();
            pool.add_component(static_cast<uint64_t>(entity), component);
        }

        template <typename Component>
        std::optional<Component>& Registry::get_component(const Entity& entity) {
            auto& pool = register_component<Component>();
            return pool.get_component(static_cast<uint64_t>(entity));
        }

        template <typename Component>
        ComponentPool<Component>& Registry::get_component_pool() {
            return register_component<Component>();
        }

        template <typename Component>
        void Registry::remove_component(Entity entity) {
            auto& pool = register_component<Component>();
            pool.remove_component(static_cast<uint64_t>(entity));
        }

        template <typename Component>
        void ComponentPool<Component>::add_component(uint64_t entity_id, const Component& component) {
            if (entity_id >= components.size()) {
                components.resize(entity_id + 1);
            }
            components[entity_id] = component;
        }

        template <typename Component>
        std::optional<Component>& ComponentPool<Component>::get_component(uint64_t entity_id) {
            if (entity_id < components.size()) {
                return components[entity_id];
            }
            throw std::out_of_range("Entity ID out of range");
        }

        template <typename Component>
        void ComponentPool<Component>::remove_component(uint64_t entity_id) {
            if (entity_id < components.size()) {
                components[entity_id].reset();
            }
        }

        template <typename Component>
        std::size_t ComponentPool<Component>::size() const {
            return components.size();
        }
    }

#endif /* !ENGINE_CORE_TPP_ */
