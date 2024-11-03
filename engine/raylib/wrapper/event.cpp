#include "engineGraphicRay.hpp"

using namespace grw;

event::event(unsigned long type, const engine_math::vector2<int> &position, int key)
    : type(type), position(position), key(key)
{
}