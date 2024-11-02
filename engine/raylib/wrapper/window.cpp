#include "engineGraphicRay.hpp"
#include <raylib.h>

using namespace grw;

/**
 * @brief Create a new window
 * 
 * @param title The title of the window
 * @param mode The video mode of the window
 */
window::window(const std::string &title, const videoMode &mode)
    : _closed(0), _title(title), _videoMode(mode)
{
    InitWindow(mode.size.x, mode.size.y, title.c_str());
    if (engine_math::bitFlag::getFlag(mode.flags, grw::videoMode::VSYNC)) {
        SetTargetFPS(60); // Assuming 60 FPS for VSYNC; adjust as needed
    }
}

window::~window()
{
    if (this->isClosed())
        return;
    this->close();
}

/**
 * @brief Close the window
 */
void window::close(void)
{
    this->_closed = 1;
    CloseWindow();  // Raylib equivalent of closing the window
}

unsigned char window::isClosed(void)
{
    return (this->_closed || WindowShouldClose());
}

void window::draw(void)
{
    if (this->isClosed())
        return;

    clear(0);
    
}

void window::clear(unsigned int color)
{
    if (this->isClosed())
        return;
    unsigned char r = color >> 24;
    unsigned char g = color >> 16;
    unsigned char b = color >> 8;
    unsigned char a = color >> 0;
    Color rayColor = {r, g, b, a};
    ClearBackground(rayColor);
}

void window::clearEvents(void)
{
    this->_events.clear();
}

/**
 * @brief Register an event
 * 
 * @param toRegister The event to register
 */
void window::registerEvent(const event &toRegister)
{
    this->_events[toRegister.type] = toRegister;
}

const event window::getEvent(unsigned long eventType) const
{
    if (this->_events.find(eventType) == this->_events.end())
        return (event(event::NO_EVENT));
    return (this->_events.at(eventType));
}

unsigned char window::hasEvent(unsigned long eventType)
{
    return (this->_events.find(eventType) != this->_events.end());
}

/**
 * @brief Update the window
 */
void window::update(void)
{
    if (engine_math::bitFlag::getFlag(this->_videoMode.flags, videoMode::CLOSABLE)) {
        if (this->hasEvent(event::CLOSE) || WindowShouldClose()) {
            this->close();
        }
    }
}

void window::addSprite(const std::string &name, const std::shared_ptr<texture> &texture)
{
    _sprites.push_back(sprite(texture, engine_math::vector2<int>(0, 0), engine_math::vector2<int>(texture->getSize().x, texture->getSize().y)));
}

void window::addSprite(const std::string &name)
{
    _sprites.push_back(sprite(std::shared_ptr<texture>(new texture(name)), engine_math::vector2<int>(0, 0), engine_math::vector2<int>(0, 0)));
}
