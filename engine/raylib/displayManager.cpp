#include "engineGraphicRay.hpp"
#include "engineComponents.hpp"
#include <iostream>

engine::displayManager::displayManager()
{

}

void engine::displayManager::registerAsset(const std::string &name, const std::string &path)
{
    this->_winHandler.getWindow(0)->addSprite(name);
}

void engine::displayManager::registerAsset(const std::string &name, const std::shared_ptr<grw::texture> &texture)
{
    this->_winHandler.getWindow(0)->addSprite(name, texture);
}

void engine::displayManager::addWindow(grw::window *window)
{
    if (!window)
        throw nullPtrError("window argument in displayManager::addWindow should not be a null ptr");
    this->_winHandler.addWindow(window);
}

unsigned int engine::displayManager::createWindow(void)
{
    grw::window *window = new grw::window("New Window", grw::videoMode(engine_math::vector2<int>(800, 600), engine_math::vector2<int>(0, 0), 32, grw::videoMode::CLOSABLE));

    SetTraceLogLevel(LOG_NONE);
    this->_winHandler.addWindow(window);
    return (0);
}

void engine::displayManager::update(void)
{
    this->_winHandler.updateWindows();
}

void engine::displayManager::draw(void)
{
    BeginDrawing();
    this->_winHandler.drawWindows();
    EndDrawing();
}

void engine::displayManager::clear(void)
{
    this->_winHandler.clearWindows();
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

    if (!position || !sprite)
        return;

}
