#include "engineGraphic.hpp"
#include "engineComponents.hpp"

engine::displayManager::displayManager()
{

}

void engine::displayManager::registerAsset(const std::string &name, const std::string &path)
{
    this->_textures[name] = std::make_shared<grw::texture>(path);
}

void engine::displayManager::registerAsset(const std::string &name, const std::shared_ptr<grw::texture> &texture)
{
    this->_textures[name] = texture;
}

void engine::displayManager::addWindow(grw::window *window)
{
    if (!window)
        throw nullPtrError("window argument in displayManager::addWindow should not be a null ptr");
    this->_winHandler.addWindow(window);
    this->_rendering[window->getId()] = std::vector<grw::sprite>();
}

unsigned int engine::displayManager::createWindow(void)
{
    grw::window *window = new grw::window("New Window", grw::videoMode(engine_math::vector2<int>(800, 600), engine_math::vector2<int>(0, 0), 32, grw::videoMode::CLOSABLE));

    this->_winHandler.addWindow(window);
    this->_rendering[window->getId()] = std::vector<grw::sprite>();
    return (window->getId());
}

void engine::displayManager::update(void)
{
    this->_winHandler.updateWindows();

    for (auto it = this->_rendering.begin(); it != this->_rendering.end(); ++it) {
        if (this->_winHandler.isWindowClosed(it->first))
            this->_rendering.erase(it->first);
    }
}

void engine::displayManager::draw(void)
{
    for (auto it = this->_rendering.begin(); it != this->_rendering.end(); ++it) {
        if (!this->_winHandler.isWindowClosed(it->first)) {
            for (const auto &entity : it->second) {
                entity.draw(this->_winHandler.getWindow(it->first)->getSurface());
            }
        }
    }

    this->_winHandler.drawWindows();
}

void engine::displayManager::clear(void)
{
    for (auto it = this->_rendering.begin(); it != this->_rendering.end(); ++it) {
        if (!this->_winHandler.isWindowClosed(it->first)) {
            auto mask = this->_winHandler.getWindow(it->first)->getSurface()->getMask();

            this->_rendering[it->first].clear();
            this->_winHandler.getWindow(it->first)->clear(mask.createColor(0x00, 0x00, 0x00, 0xff));
        }
    }
}

bool engine::displayManager::event(void)
{
    this->_winHandler.eventWindows();

    return (this->_winHandler.isAllWindowClosed());
}

void engine::displayManager::useEntity(const engine::Entity &entity, engine::Registry &registry)
{
    auto &position = registry.get_component<engine_components::Position>(entity);
    auto &sprite = registry.get_component<engine_components::Sprite>(entity);

    if (!position || !sprite)
        return;

    grw::sprite newSprite(this->_textures[sprite->name], position->coordinates, engine_math::vector2<int>(-1, -1));
}
