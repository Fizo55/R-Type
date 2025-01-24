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
    #include <any>
    #include <functional>

    #include <yaml-cpp/yaml.h>

    #include <boost/archive/text_oarchive.hpp>
    #include <boost/archive/text_iarchive.hpp>
    #include <boost/serialization/map.hpp>
    #include <boost/serialization/vector.hpp>
    #include <boost/serialization/string.hpp>
    #include <boost/serialization/unique_ptr.hpp>
    #include <boost/serialization/optional.hpp>
    #include <boost/serialization/access.hpp>

    namespace engine {
        struct EcsSystem;
        struct ComponentPoolBase;
        struct Entity;
        struct Registry;
        template <typename Component>
        struct ComponentPool;

        class Entity {
            public:
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

        class ComponentBuildRoute {
            public:
                ComponentBuildRoute(const std::string & = "", const std::function<void(const Entity &, const std::string &, const std::vector<std::any> &, Registry &)> & = std::function<void(const Entity &, const std::string &, const std::vector<std::any> &, Registry &)>());
                ~ComponentBuildRoute() = default;

                std::string name;
                std::function<void(const Entity &, const std::string &, const std::vector<std::any> &, Registry &)> callback;
        };

        class EntityBuildData {
            public:
                EntityBuildData(const std::string &, const std::string &, const std::vector<std::any> &);
                ~EntityBuildData() = default;

                std::string nameRef;
                std::string name;
                std::vector<std::any> buildArgs;
        };

        class EntityFactory {
            public:
                EntityFactory();
                ~EntityFactory() = default;

                template <typename Component>
                void registerComponent() { this->_registry.register_component<Component>(); };

                void registerBuildComponentRoute(const ComponentBuildRoute &);

                void buildComponent(const Entity &, const EntityBuildData &);

                Entity createEntityComponentReady(const std::vector<EntityBuildData> &);
                Entity createEntity(void);

                Registry &getRegistry(void);

            private:
                Registry _registry;

                std::map<std::string, ComponentBuildRoute> _routes;
        };

        class ObjectRef {
            public:
                ObjectRef();
                ObjectRef(const std::string &);

                friend class boost::serialization::access;

                template <class Archive>
                void serialize_any(Archive& ar, std::any& a, const unsigned int version) {
                    if (a.type() == typeid(int)) {
                        int value = std::any_cast<int>(a);
                        ar & value;
                        a = value;
                    } else if (a.type() == typeid(double)) {
                        double value = std::any_cast<double>(a);
                        ar & value;
                        a = value;
                    } else if (a.type() == typeid(std::string)) {
                        std::string value = std::any_cast<std::string>(a);
                        ar & value;
                        a = value;
                    } else {
                        throw std::runtime_error("Unsupported type for serialization");
                    }
                }

                template <class Archive>
                void serialize(Archive& ar, const unsigned int version) {
                    ar & _name;
                    ar & _buildComponents;

                    for (auto& [key, value_list] : _buildParameters) {
                        ar & key;
                        for (auto& value : value_list) {
                            serialize_any(ar, value, version);
                        }
                    }
                }

                const std::string &getName(void) const;
                void setName(const std::string &);

                void addBuildComponent(const std::string &, const std::string &);
                void removeBuildComponent(const std::string &);
                const std::string &getBuildComponent(const std::string &) const;
                std::vector<std::string> getAllBuildComponent(void) const;

                void addBuildParameter(const std::string &, const std::vector<std::any> &);
                void removeBuildParameter(const std::string &);
                const std::vector<std::any> getBuildParameter(const std::string &) const;
                std::vector<std::string> getAllBuildParameter(void) const;

                static std::any parameterBuilder(const YAML::Node &);

            private:
                std::string _name;
                std::map<std::string, std::string> _buildComponents;
                std::map<std::string, std::vector<std::any>> _buildParameters;
        };

        class Object : public ObjectRef {
            public:
                Object();
                Object(const ObjectRef &);

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive & ar, const unsigned int version) {
                    ar & boost::serialization::base_object<ObjectRef>(*this);

                    if (Archive::is_saving::value) {
                        if (_entity) {
                            std::size_t entityId = static_cast<std::size_t>(*_entity);
                            ar & entityId;
                        } else {
                            std::size_t invalidId = std::numeric_limits<std::size_t>::max();
                            ar & invalidId;
                        }
                    } else {
                        std::size_t entityId;
                        ar & entityId;
                        if (entityId != std::numeric_limits<std::size_t>::max()) {
                            // idk
                        } else {
                            _entity.reset();
                        }
                    }
                }

                void buildEntity(EntityFactory *);

                const std::unique_ptr<Entity> &getEntity(void) const;

                std::vector<char> serializeToBytes() const;

                static Object *deserializeFromBytes(const std::string& buffer, EntityFactory *factory);
            private:
                std::unique_ptr<Entity> _entity;
        };

        class Scene {
            public:
                Scene();
                Scene(const std::string &);

                const std::string &getName(void) const;
                void setName(const std::string &);

                void addHud(const std::string &, const ObjectRef &);
                void addObject(const std::string &, const ObjectRef &);

                const std::vector<std::pair<std::string, ObjectRef>> &getHuds(void) const;
                const std::vector<std::pair<std::string, ObjectRef>> &getObjects(void) const;

            private:
                std::string _name;
                std::vector<std::pair<std::string, ObjectRef>> _objects;
                std::vector<std::pair<std::string, ObjectRef>> _huds;
        };

        class GameDB {
            public:
                GameDB();
                ~GameDB();

                std::optional<std::any> readDB(std::size_t);
                void writeDB(std::size_t, const std::any &);

            private:
                std::map<std::size_t, std::any> _dbRegister;
        };

        class Game {
            public:
                Game();
                ~Game();

                const Scene &getScene(const std::string &) const;
                const ObjectRef &getObject(const std::string &) const;

                void loadObject(Object *obj);
                void loadHud(Object *obj);

                void unloadAllObjects(void);

                void unloadObject(engine::Object *obj);
                void unloadHud(engine::Object *obj);

                void registerScene(const std::string &);
                void registerScene(const Scene &);

                void registerObject(const std::string &);
                void registerObject(const ObjectRef &);

                void unregisterScene(const std::string &);
                void unregisterObject(const std::string &);

                void loadScene(const std::string &);
                void loadSceneClient(const std::string &);
                void unloadScene(void);

                const std::vector<Object *> &getLoadedObjects(void);
                const std::vector<Object *> &getLoadedHuds(void);

                void addFactory(EntityFactory *);
                engine::EntityFactory *getFactory(void);

                std::int64_t readDBInt(std::size_t);
                void writeDBInt(std::size_t, std::int64_t);

                Object *buildObjectRef(const ObjectRef &, const std::string &);

            private:
                std::map<std::string, ObjectRef> _objects;
                std::map<std::string, Scene> _scenes;

                std::vector<Object *> _loadedGameObjects;
                std::vector<Object *> _loadedGameHuds;

                EntityFactory *_factory;

                GameDB _db;

                std::string _loadedScene;
        };
    }

    std::ostream &operator<<(std::ostream &os, engine::ObjectRef const &obj);
    std::ostream &operator<<(std::ostream &os, engine::Object const &obj);

    #include "engineCore.tpp"

#endif /* !ENGINE_CORE_HPP_ */