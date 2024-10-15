#include "engineGraphic.hpp"

using namespace grw;

/**
 * @brief Create a new video mode
 * 
 * @param size The size of the video mode
 * @param position The position of the video mode
 * @param depht The depht of the video mode
 * @param flags The flags of the video mode
*/
videoMode::videoMode(const engine_math::vector2<int> &size, const engine_math::vector2<int> &position, unsigned int depht, unsigned long flags)
    : size(size), position(position), depht(depht), flags(flags)
{

}
