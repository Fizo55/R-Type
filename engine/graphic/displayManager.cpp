#include "engineGraphic.hpp"

engine::displayManager::displayManager()
{

}

void engine::displayManager::registerAsset(const std::string &name, const std::string &path)
{
    this->textures[name] = grw::texture(path);
}

void engine::displayManager::registerAsset(const std::string &name, const grw::texture &texture)
{
    this->textures[name] = texture;
}
