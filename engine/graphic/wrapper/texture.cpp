#include "engineGraphic.hpp"
#include "engineExceptions.hpp"

using namespace grw;


texture::texture(const engine_math::vector2<int> &size, unsigned int depht, const mask &textureMask)
    : _texture(SDL_CreateRGBSurface(0, size.x, size.y, depht, textureMask.r, textureMask.g, textureMask.b, textureMask.a)), _size(size), _depht(depht), _mask(textureMask)
{
    if (!this->_texture)
        throw engine::outOfMemoryError("out of memory, failed to create texture of size " + std::to_string(size.x) + " x " + std::to_string(size.y) + ".");
}

texture::texture(SDL_Surface *surface)
    : _texture(surface), _size(engine_math::vector2<int>(0, 0))
{
    if (!surface)
        throw (engine::nullPtrError("nullptr not allowrd as pointer type."));
    this->_size.x = surface->w;
    this->_size.y = surface->h;
    this->_depht = surface->format->BitsPerPixel;
    this->_mask = mask(surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
}

texture::texture(const std::string &filePath)
    : _texture(IMG_Load(filePath.c_str())), _size(engine_math::vector2<int>(0, 0))
{
    if (!this->_texture)
        throw (engine::loadError("failed to load texture from filepath: " + filePath + "."));
    this->_size.x = this->_texture->w;
    this->_size.y = this->_texture->h;
    this->_depht = this->_texture->format->BitsPerPixel;
    this->_mask = mask(this->_texture->format->Rmask, this->_texture->format->Gmask, this->_texture->format->Bmask, this->_texture->format->Amask);
}

void texture::blit(const texture &other, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
{
    SDL_Rect tempOther = {0, 0, size.x >= 0 ? size.x : other.getSize().x, size.y >= 0 ? size.y : other.getSize().y};
    SDL_Rect temp = {position.x, position.y, this->_size.x, this->_size.y};

    SDL_BlitSurface(other.getSurface(), &tempOther, this->_texture, &temp);
}

void texture::draw_rect(const engine_math::vector2<int> &position, const engine_math::vector2<int> &size, unsigned int color)
{
    SDL_Rect temp = {position.x, position.y, size.x, size.y};

    SDL_FillRect(this->_texture, &temp, color);
}

void texture::clear(unsigned int color)
{
    SDL_Rect temp = {0, 0, this->_texture->w, this->_texture->h};

    SDL_FillRect(this->_texture, &temp, color);
}

void texture::optimize(const texture &other)
{
    SDL_ConvertSurface(this->_texture, other.getSurface()->format, 0);

    this->_depht = this->_texture->format->BitsPerPixel;
    this->_mask = mask(this->_texture->format->Rmask, this->_texture->format->Gmask, this->_texture->format->Bmask, this->_texture->format->Amask);
}

SDL_Surface *texture::getSurface(void) const
{
    return (this->_texture);
}

const engine_math::vector2<int> &texture::getSize(void) const
{
    return (this->_size);
}

texture::~texture()
{
    if (this->_texture) {
        SDL_FreeSurface(this->_texture);
        this->_texture = nullptr;
    }
}
