#include "engineGraphic.hpp"

using namespace grw;

videoMode::videoMode(const engine_math::vector2<int> &size, const engine_math::vector2<int> &position, unsigned int depht, unsigned long flags)
    : size(size), position(position), depht(depht), flags(flags)
{

}