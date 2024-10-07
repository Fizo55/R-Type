#include "engineComponents.hpp"
#include "engineExceptions.hpp"

engine_components::Position::Position(long x, long y)
  :  coordinates(engine_math::vector2<long>(x, y))
{

}

engine_components::Position::~Position()
{

}

void engine_components::Position::buildComponent(const engine::Entity &entity, const std::vector<std::any> &args, engine::Registry &registry)
{
    std::vector<long> newVec;

    if (args.size() > 2)
        throw engine::componentBuildError("too many argument in componentBuild for Position.");
    for (std::size_t i = 0; i < 2; ++i) {
        if (args.size() > (i + 1))
            newVec.push_back(std::any_cast<long>(args[i]));
        else
            newVec.push_back(0);
    }
    registry.add_component(entity, Position{newVec[0], newVec[1]});
}
