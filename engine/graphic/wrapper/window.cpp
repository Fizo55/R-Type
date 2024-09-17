#include "engineGraphic.hpp"

using namespace grw;

window::window(const std::string &title, const videoMode &mode)
    : _title(title), _videoMode(mode)
{
    this->_window = SDL_CreateWindow(title.c_str(), mode.position.x, mode.position.y, mode.size.x, mode.size.y, 0);
    this->_texture = std::make_unique<texture>(SDL_GetWindowSurface(this->_window));
}

window::~window()
{
    this->_texture.release();
    SDL_DestroyWindow(this->_window);
}

void window::draw()
{
    SDL_UpdateWindowSurface(this->_window);
}
