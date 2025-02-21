#include "engineGraphic.hpp"
#include "engineComponents.hpp"
#include <SDL2/SDL_ttf.h>

engine::displayManager::displayManager()
{
    TTF_Init();
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

unsigned int engine::displayManager::createWindow(int x, int y)
{
    grw::window *window = new grw::window("New Window", grw::videoMode(engine_math::vector2<int>(x, y), engine_math::vector2<int>(0, 0), 32, grw::videoMode::CLOSABLE));

    this->_winHandler.addWindow(window);
    this->_rendering[window->getId()] = std::vector<grw::sprite>();
    return (window->getId());
}

void engine::displayManager::update(void)
{
    this->_winHandler.updateWindows();

    for (auto it = this->_rendering.begin(); it != this->_rendering.end();) {
        if (this->_winHandler.isWindowClosed(it->first)) {
            it = this->_rendering.erase(it);
        } else {
            ++it;
        }
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

bool engine::displayManager::hasEvent(unsigned int window, grw::event::eventTypes event)
{
    if (this->_winHandler.hasWindow(window))
        return (this->_winHandler.getWindow(window)->hasEvent(event));

    return false;
}

grw::event engine::displayManager::getEvent(unsigned int window, grw::event::eventTypes event)
{
    if (this->_winHandler.hasWindow(window))
        return (this->_winHandler.getWindow(window)->getEvent(event));

    return (grw::event());
}

bool engine::displayManager::event(void)
{
    this->_winHandler.eventWindows();

    return (this->_winHandler.isAllWindowClosed());
}

void engine::displayManager::useEntity(const engine::Entity &entity, engine::Registry &registry, unsigned int windowId)
{
    auto &position = registry.get_component<engine_components::Position>(entity);
    auto &sprite = registry.get_component<engine_components::Sprite>(entity);
    auto &size = registry.get_component<engine_components::Size>(entity);

    if (!position || !sprite || !size)
        return;

    grw::sprite newSprite(this->_textures[sprite->sprite], position->coordinates, size->coordinates);
    this->_rendering[windowId].push_back(newSprite);
}

void engine::displayManager::useText(const std::string &text, const std::string &font_path, int font_size, const engine_math::vector2<int> &pos, const engine_math::vector2<int> &size, unsigned int windowId)
{
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);

    grw::sprite newSprite(std::make_shared<grw::texture>(TTF_RenderText_Solid(font, text.c_str(), {255, 255, 255})), pos, size);
    this->_rendering[windowId].push_back(newSprite);
    TTF_CloseFont(font);
}
