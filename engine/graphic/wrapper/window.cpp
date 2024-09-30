#include "engineGraphic.hpp"

using namespace grw;

window::window(const std::string &title, const videoMode &mode)
    : _closed(0), _title(title), _videoMode(mode)
{
    this->_window = SDL_CreateWindow(title.c_str(), mode.position.x, mode.position.y, mode.size.x, mode.size.y, 0);

    if (!this->_window)
        throw engine::outOfMemoryError("out of memory, failed to create window.");

    this->_renderer = SDL_CreateRenderer(this->_window, -1, ((engine_math::bitFlag::getFlag(mode.flags, grw::videoMode::VSYNC) ? SDL_RENDERER_PRESENTVSYNC : 0) | ((engine_math::bitFlag::getFlag(mode.flags, grw::videoMode::ACCELERATION)) ? SDL_RENDERER_ACCELERATED : 0)));
    if (!this->_renderer)
        throw engine::outOfMemoryError("out of memory, failed to create renderer.");

    this->_id = SDL_GetWindowID(this->_window);
    this->_texture = std::make_unique<texture>(SDL_GetWindowSurface(this->_window));
}

window::~window()
{
    if (this->isClosed())
        return;
    this->_closed = 1;
    this->_texture.release();

    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
}

void window::close(void)
{
    this->_closed = 1;
    this->_texture.release();

    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
}

unsigned char window::isClosed(void)
{
    return (this->_closed);
}

void window::draw(void)
{
    if (this->isClosed())
        return;
    SDL_UpdateWindowSurface(this->_window);
}

void window::clear(unsigned int color)
{
    if (this->isClosed())
        return;
    this->_texture->clear(color);
}

void window::blit(const texture &other, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
{
    if (this->isClosed())
        return;
    this->_texture->blit(other, position, size);
}

void window::clearEvents(void)
{
    this->_events.clear();
}

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

void window::update(void)
{
    if (engine_math::bitFlag::getFlag(this->_videoMode.flags, videoMode::CLOSABLE)) {
        if (this->hasEvent(event::CLOSE) || this->hasEvent(event::QUIT)) {
            this->close();
        }
    }
}

int window::getId(void) const
{
    return (this->_id);
}
