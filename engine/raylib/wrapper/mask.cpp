#include "engineGraphicRay.hpp"

using namespace grw;

/**
 * @brief Create a new mask
 * 
 * @param r The red mask
 * @param g The green mask
 * @param b The blue mask
 * @param a The alpha mask
 */
mask::mask(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
    : r(r), g(g), b(b), a(a)
{

}

/**
 * @brief Create a new color with the mask
 * 
 * @param rColor The red color
 * @param gColor The green color
 * @param bColor The blue color
 * @param aColor The alpha color
 * @return unsigned The color
 */
unsigned mask::createColor(unsigned char rColor, unsigned char gColor, unsigned char bColor, unsigned char aColor)
{
    unsigned int rColorTemp = 0x00;
    unsigned int gColorTemp = 0x00;
    unsigned int bColorTemp = 0x00;
    unsigned int aColorTemp = 0x00;

    *((unsigned char *)&rColorTemp) = rColor;
    *((unsigned char *)&rColorTemp + 1) = rColor;
    *((unsigned char *)&rColorTemp + 2) = rColor;
    *((unsigned char *)&rColorTemp + 3) = rColor;

    *((unsigned char *)&gColorTemp) = gColor;
    *((unsigned char *)&gColorTemp + 1) = gColor;
    *((unsigned char *)&gColorTemp + 2) = gColor;
    *((unsigned char *)&gColorTemp + 3) = gColor;

    *((unsigned char *)&bColorTemp) = bColor;
    *((unsigned char *)&bColorTemp + 1) = bColor;
    *((unsigned char *)&bColorTemp + 2) = bColor;
    *((unsigned char *)&bColorTemp + 3) = bColor;

    *((unsigned char *)&aColorTemp) = aColor;
    *((unsigned char *)&aColorTemp + 1) = aColor;
    *((unsigned char *)&aColorTemp + 2) = aColor;
    *((unsigned char *)&aColorTemp + 3) = aColor;

    return ((this->r & rColorTemp) | (this->g & gColorTemp) | (this->b & bColorTemp) | (this->a & aColorTemp));
}

/**
 * @brief Get the red color from a color
 * 
 * @param color The color
 * @return unsigned char The red color
 */ 
unsigned char mask::getR(unsigned int color)
{
    color = color & this->r;
    while (color > 255)
        color >>= 8;
    return ((unsigned char)color);
}

/**
 * @brief Get the green color from a color
 * 
 * @param color The color
 * @return unsigned char The green color
 */
unsigned char mask::getG(unsigned int color)
{
    color = color & this->g;
    while (color > 255)
        color >>= 8;
    return ((unsigned char)color);
}

/**
 * @brief Get the blue color from a color
 * 
 * @param color The color
 * @return unsigned char The blue color
 */
unsigned char mask::getB(unsigned int color)
{
    color = color & this->b;
    while (color > 255)
        color >>= 8;
    return ((unsigned char)color);
}

/**
 * @brief Get the alpha color from a color
 * 
 * @param color The color
 * @return unsigned char The alpha color
 */
unsigned char mask::getA(unsigned int color)
{
    color = color & this->a;
    while (color > 255)
        color >>= 8;
    return ((unsigned char)color);
}
