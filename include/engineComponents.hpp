#ifndef ENGINE_COMPONENTS_HPP_
    #define ENGINE_COMPONENTS_HPP_

    #include <iostream>
    #include <vector>
    #include <any>

    #include "engineCore.hpp"
    #include "engineMath.hpp"

    namespace engine_components {
        class IComponent {
            public:
                IComponent() = default;
                virtual ~IComponent() = default;

                std::string name;
        };

        class Position : public IComponent {
            public:
                Position(const std::string &, int x, int y);
                ~Position();

                void static buildComponent(const engine::Entity &, const std::string &, const std::vector<std::any> &, engine::Registry &);

                engine_math::vector2<int> coordinates;
        };

        class Size : public IComponent {
            public:
                Size(const std::string &, int x, int y);
                ~Size();

                void static buildComponent(const engine::Entity &, const std::string &, const std::vector<std::any> &, engine::Registry &);

                engine_math::vector2<int> coordinates;
        };

        class Sprite : public IComponent {
            public:
                Sprite(const std::string &, const std::string &);
                ~Sprite();

                void static buildComponent(const engine::Entity &, const std::string &, const std::vector<std::any> &, engine::Registry &);

                std::string sprite;
        };

        class Script : public IComponent {
            public:
                Script(const std::string &, const std::string &);
                ~Script();

                void static buildComponent(const engine::Entity &, const std::string &, const std::vector<std::any> &, engine::Registry &);

                std::string script;
        };

        class Volatile : public IComponent {
            public:
                Volatile(const std::string &);
                ~Volatile();

                void static buildComponent(const engine::Entity &, const std::string &, const std::vector<std::any> &, engine::Registry &);
        };
    };

#endif /* ENGINE_COMPONENTS_HPP_ */
