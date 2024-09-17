#include "engineGraphic.hpp"

using namespace grw;


texture::texture(const engine_math::vector2<int> &size, unsigned int depht, unsigned char alpha)
    : _texture(SDL_CreateRGBSurface(0, size.x, size.y, depht, 0xff, 0xff, 0xff, alpha ? 0xff : 0))
{

}

texture::texture(SDL_Surface *surface)
    : _texture(surface)
{

}

texture::~texture()
{
    SDL_FreeSurface(this->_texture);
}
