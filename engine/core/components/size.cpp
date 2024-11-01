#include "engineComponents.hpp"
#include "engineExceptions.hpp"

engine_components::Size::Size(const std::string &name, int x, int y)
  : coordinates(engine_math::vector2<int>(x, y))
{
    this->name = name;
}

engine_components::Size::~Size()
{

}

void engine_components::Size::buildComponent(const engine::Entity &entity, const std::string &name, const std::vector<std::any> &args, engine::Registry &registry)
{
    std::vector<int> newVec;

    if (args.size() > 2)
        throw engine::componentBuildError("too many argument in componentBuild for Size.");
    for (std::size_t i = 0; i < 2; ++i) {
        if (args.size() > (i))
            newVec.push_back(std::any_cast<std::int64_t>(args[i]));
        else
            newVec.push_back(0);
    }
    registry.add_component(entity, Size{name, newVec[0], newVec[1]});
}
