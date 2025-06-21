#include "actors/ParallaxBackground.hpp"

void ParallaxBackground::draw()
{

    al_draw_bitmap(backgroundImage, x, y, 0);
    if (x < 0)
    {
        al_draw_bitmap(backgroundImage, x + width, y, 0);
    }
}

void ParallaxBackground::update(float deltaTime)
{
    x -= speed * deltaTime;
    if (x + width < 0)
    {
        x = 0;
    }
}