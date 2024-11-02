#include "engineGraphicRay.hpp"
#include "engineExceptions.hpp"
#include <filesystem>

using namespace grw;

/**
 * @brief Create a new texture
 * 
 * @param size The size of the texture
 */
texture::texture(const engine_math::vector2<int> &size)
    : _size(size)
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
    // Verify graphics context initialization
    if (!IsWindowReady()) {
        std::cerr << "Window is not initialized. Please initialize the window before loading textures." << std::endl;
        return;
    }

    // Verify file existence
    if (!std::filesystem::exists(filePath)) {
        throw engine::loadError("Texture file does not exist: " + filePath);
    }


    // Load texture
    _texture = LoadTexture(filePath.c_str());

    // Check for successful loading
    if (_texture.id == 0) {
        throw engine::loadError("Failed to load texture from filepath: " + filePath + ".");
    }

    _size = engine_math::vector2<int>(_texture.width, _texture.height);
}

/**
 * @brief Blit (draw) a texture on another texture
 * 
 * @param x The x position to draw the texture
 * @param y The y position to draw the texture
 */
void texture::draw(int x, int y)
{
    DrawTexture(_texture, x, y, WHITE);
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
    Color rayColor = {};

    DrawRectangle(position.x, position.y, size.x, size.y, rayColor);
}

/**
 * @brief Clear the texture with a color
 * 
 * @param color The color to clear the texture with
 */
void texture::clear(unsigned int color)
{
    Color rayColor = {};
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
