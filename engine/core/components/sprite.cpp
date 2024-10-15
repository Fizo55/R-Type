#include "engineComponents.hpp"
#include "engineExceptions.hpp"

engine_components::Sprite::Sprite(const std::string &name, const std::string &sprite)
  : sprite(sprite)
{
    this->name = name;
}

engine_components::Sprite::~Sprite()
{

}

void engine_components::Sprite::buildComponent(const engine::Entity &entity, const std::string &name, const std::vector<std::any> &args, engine::Registry &registry)
{
    std::vector<std::string> newVec;

    if (args.size() > 1)
        throw engine::componentBuildError("too many argument in componentBuild for Sprite.");
    for (std::size_t i = 0; i < 1; ++i) {
        if (args.size() > (i))
            newVec.push_back(std::any_cast<std::string>(args[i]));
        else
            newVec.push_back("");
    }
    registry.add_component(entity, Sprite{name, newVec[0]});
}
