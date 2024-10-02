/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** sprite
*/

#include <string>
#include "engineGraphic.hpp"

sprite::sprite(const std::string &filePath, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
    : _spriteTexture(filePath), _position(position), _size(size) {}

sprite::sprite(SDL_Surface *surface, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
    : _spriteTexture(surface), _position(position), _size(size) {}

sprite::~sprite() {}

void sprite::setPosition(const engine_math::vector2<int> &position)
{
    this->_position = position;
}

void sprite::setSize(const engine_math::vector2<int> &size)
{
    this->_size = size;
}

const engine_math::vector2<int> &sprite::getPosition() const
{
    return this->_position;
}

const engine_math::vector2<int> &sprite::getSize() const
{
    return this->_size;
}

texture *getTexture() const
{
    return this->_texture;
}

void sprite::draw(texture &texture, const engine_math::vector2<int> &position) const {
    texture.blit(_spriteTexture, position, this->_size);
}