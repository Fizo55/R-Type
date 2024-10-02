#include "engineGraphic.hpp"
#include "engineExceptions.hpp"

using namespace grw;

/**
 * @brief Create a new texture
 * 
 * @param size The size of the texture
 * @param depht The depht of the texture
 * @param textureMask The mask of the texture
 */
texture::texture(const engine_math::vector2<int> &size, unsigned int depht, const mask &textureMask)
    : _texture(SDL_CreateRGBSurface(0, size.x, size.y, depht, textureMask.r, textureMask.g, textureMask.b, textureMask.a)), _size(size), _depht(depht), _mask(textureMask)
{
    if (!this->_texture)
        throw engine::outOfMemoryError("out of memory, failed to create texture of size " + std::to_string(size.x) + " x " + std::to_string(size.y) + ".");
}

/**
 * @brief Create a new texture from a SDL_Surface
 * 
 * @param surface The SDL_Surface to create the texture from
 */
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

/**
 * @brief Create a new texture from a file
 * 
 * @param filePath The path to the file
 */
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

/**
 * @brief Blit a texture on another texture
 * 
 * @param other The texture to blit
 * @param position The position to blit the texture
 * @param size The size of the texture to blit
 */
void texture::blit(const texture &other, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
{
    SDL_Rect tempOther = {0, 0, size.x >= 0 ? size.x : other.getSize().x, size.y >= 0 ? size.y : other.getSize().y};
    SDL_Rect temp = {position.x, position.y, this->_size.x, this->_size.y};

    SDL_BlitSurface(other.getSurface(), &tempOther, this->_texture, &temp);
}

/**
 * @brief Draw a rectangle on the texture
 * 
 * @param position The position of the rectangle
 * @param size The size of the rectangle
 * @param color The color of the rectangle
 */
void texture::draw_rect(const engine_math::vector2<int> &position, const engine_math::vector2<int> &size, unsigned int color)
{
    SDL_Rect temp = {position.x, position.y, size.x, size.y};

    SDL_FillRect(this->_texture, &temp, SDL_MapRGBA(this->_texture->format, this->_mask.getR(color), this->_mask.getG(color), this->_mask.getB(color), this->_mask.getA(color)));
}

/**
 * @brief Clear the texture with a color
 * 
 * @param color The color to clear the texture with
 */
void texture::clear(unsigned int color)
{
    SDL_FillRect(this->_texture, (const SDL_Rect *)0, SDL_MapRGBA(this->_texture->format, this->_mask.getR(color), this->_mask.getG(color), this->_mask.getB(color), this->_mask.getA(color)));
}

void texture::setMask(const mask &mask)
{
    this->_mask = mask;

    this->_texture->format->Rmask = mask.r;
    this->_texture->format->Gmask = mask.g;
    this->_texture->format->Bmask = mask.b;
    this->_texture->format->Amask = mask.a;
}

std::unique_ptr<texture> &window::getSurface(void)
{
    return (this->_texture);
}

const std::unique_ptr<texture> &window::getSurface(void) const
{
    return (this->_texture);
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
