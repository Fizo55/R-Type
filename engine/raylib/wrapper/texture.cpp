#include "engineGraphicRay.hpp"
#include "engineExceptions.hpp"
#include <raylib.h>

using namespace grw;

/**
 * @brief Create a new texture
 * 
 * @param size The size of the texture
 * @param depth The depth of the texture
 * @param textureMask The mask of the texture
 */
texture::texture(const engine_math::vector2<int> &size, unsigned int depth, const mask &textureMask)
    : _size(size), _depth(depth), _mask(textureMask)
{
    Image image = GenImageColor(size.x, size.y, BLANK);  // Create a blank image in Raylib
    _texture = LoadTextureFromImage(image);              // Convert image to a texture
    UnloadImage(image);                                  // Free the image after creating the texture

    if (_texture.id == 0)
        throw engine::outOfMemoryError("Failed to create texture of size " + std::to_string(size.x) + " x " + std::to_string(size.y) + ".");
}

/**
 * @brief Create a new texture from a file
 * 
 * @param filePath The path to the file
 */
texture::texture(const std::string &filePath)
    : _size(engine_math::vector2<int>(0, 0))
{
    _texture = LoadTexture(filePath.c_str());
    if (_texture.id == 0)
        throw engine::loadError("Failed to load texture from filepath: " + filePath + ".");

    _size = engine_math::vector2<int>(_texture.width, _texture.height);
    _depth = 32; // Raylib uses 32-bit textures by default
    _mask = mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);  // Standard RGBA mask
}

/**
 * @brief Blit (draw) a texture on another texture
 * 
 * @param other The texture to blit
 * @param position The position to blit the texture
 * @param size The size of the texture to blit
 */
void texture::blit(const texture &other, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
{
    Rectangle source = {0, 0, static_cast<float>(size.x), static_cast<float>(size.y)};
    Rectangle dest = {static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(size.x), static_cast<float>(size.y)};
    Vector2 origin = {0.0f, 0.0f};

    DrawTexturePro(other.getTexture(), source, dest, origin, 0.0f, WHITE);
    EndTextureMode();
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
    Color rayColor = {(unsigned char)_mask.getR(color), (unsigned char)_mask.getG(color), (unsigned char)_mask.getB(color), (unsigned char)_mask.getA(color)};

    DrawRectangle(position.x, position.y, size.x, size.y, rayColor);
    EndTextureMode();
}

/**
 * @brief Clear the texture with a color
 * 
 * @param color The color to clear the texture with
 */
void texture::clear(unsigned int color)
{
    Color rayColor = {(unsigned char)_mask.getR(color), (unsigned char)_mask.getG(color), (unsigned char)_mask.getB(color), (unsigned char)_mask.getA(color)};

    ClearBackground(rayColor);
    EndTextureMode();
}

void texture::setMask(const mask &textureMask)
{
    _mask = textureMask;
}

std::shared_ptr<texture> &window::getSurface(void)
{
    return _texture;
}

const std::shared_ptr<texture> &window::getSurface(void) const
{
    return _texture;
}

/**
 * @brief Optimize the texture by converting it to the format of another texture
 * 
 * @param other The other texture to match the format
 */
void texture::optimize(const texture &other)
{
    // Raylib doesn't provide direct format optimization like SDL,
    // but this would be where such code goes if necessary.
}

Texture2D texture::getTexture(void) const
{
    return _texture;
}

const engine_math::vector2<int> &texture::getSize(void) const
{
    return _size;
}

texture::~texture()
{
    if (_texture.id != 0) {
        UnloadTexture(_texture);
    }
}

const mask &texture::getMask(void) const
{
    return _mask;
}
