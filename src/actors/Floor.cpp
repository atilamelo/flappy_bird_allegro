#include <actors/Floor.hpp>
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

Floor::Floor(ALLEGRO_BITMAP *texture) : GameObject(0, PLAYABLE_AREA_HEIGHT, BUFFER_W, FLOOR_HEIGHT)
{
    if (!texture) {
        throw std::runtime_error("Floor texture is null: failed to initialize Floor object.");
    }
    this->texture = texture;
}

void Floor::draw() {
    al_draw_bitmap(texture, x, y, 0);
    if(x < 0){
        al_draw_bitmap(texture, x + width, y, 0);
    }
}

void Floor::update(float deltaTime) {

    x -= (BACKGROUND_SCROLL_SPEED + 60.0) * deltaTime;

    if (x + width < 0) { 
        x = 0;
    }
}
