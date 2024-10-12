/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** sprite
*/

#include <string>
#include "engineGraphic.hpp"

grw::sprite::sprite(const std::shared_ptr<grw::texture> &tex, const engine_math::vector2<int> &position, const engine_math::vector2<int> &size)
    : _spriteTexture(tex), _position(position), _size(size) {}

grw::sprite::~sprite() {}

void grw::sprite::setPosition(const engine_math::vector2<int> &position)
{
    this->_position = position;
}

void grw::sprite::setSize(const engine_math::vector2<int> &size)
{
    this->_size = size;
}

const engine_math::vector2<int> &grw::sprite::getPosition() const
{
    return this->_position;
}

const engine_math::vector2<int> &grw::sprite::getSize() const
{
    return this->_size;
}

std::shared_ptr<grw::texture> &grw::sprite::getTexture()
{
    return this->_spriteTexture;
}

void grw::sprite::draw(const std::shared_ptr<grw::texture> &target) const {
    target->blit(*this->_spriteTexture, this->_position, this->_size);
}
