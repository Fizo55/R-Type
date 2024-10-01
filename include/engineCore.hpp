#ifndef ENGINE_CORE_HPP_
    #define ENGINE_CORE_HPP_

    #include <iostream>
    #include <map>
    #include <memory>
    #include <vector>
    #include <cstdint>
    #include <stdexcept>
    #include <typeindex>
    #include <unordered_map>
    #include <optional>

    namespace engine {
        struct EcsSystem;
        struct ComponentPoolBase;
        struct Entity;
        struct Registry;
        template <typename Component>
        struct ComponentPool;

        class Entity {
            public:
                Entity(const Entity&) = delete;
                Entity& operator=(const Entity&) = delete;

                explicit Entity(std::size_t id);

                explicit operator std::size_t() const;

                void attach_system(std::unique_ptr<EcsSystem> system);

                void update_systems(Registry& registry, float delta_time);

            private:
                std::size_t _id;
                std::vector<std::unique_ptr<EcsSystem>> _systems;
        };

        class EcsSystem {
            public:
                virtual ~EcsSystem() = default;

                virtual void update(Registry& registry, Entity& entity, float delta_time) = 0;
        };

        class ComponentPoolBase {
        public:
            virtual ~ComponentPoolBase() = default;

            virtual void remove_component(uint64_t entity_id) = 0;
        };

        class Registry {
            public:
                Entity create_entity();

                void destroy_entity(Entity entity);

                template <typename Component>
                ComponentPool<Component>& register_component();

                template <typename Component>
                void add_component(const Entity& entity, const Component& component);

                template <typename Component>
                std::optional<Component>& get_component(const Entity& entity);

                template <typename Component>
                ComponentPool<Component>& get_component_pool();

                template <typename Component>
                void remove_component(Entity entity);

            private:
                uint64_t _next_entity_id = 0;
                std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>> _component_pools;
        };

        template <typename Component>
        class ComponentPool : public ComponentPoolBase {
            public:
                void add_component(uint64_t entity_id, const Component& component);

                std::optional<Component>& get_component(uint64_t entity_id);

                void remove_component(uint64_t entity_id) override;

                std::size_t size() const;

            private:
                std::vector<std::optional<Component>> components;
        };

        class Scene {
            public:
                Scene();
                Scene(const std::string &);

            private:
                std::string _name;
                std::vector<std::unique_ptr<Entity>> _objects;
                std::vector<std::unique_ptr<Entity>> _huds;
        };

        class Game {
            public:
                Game();

            private:
                std::vector<std::unique_ptr<Entity>> _objects;
                std::vector<std::unique_ptr<Entity>> _huds;
                std::map<std::string, std::unique_ptr<Scene>> _scenes;
                std::string _loadedScene;
        };
    }

    #include "engineCore.tpp"

#endif /* !ENGINE_CORE_HPP_ */