/**
 * @file Pipe.cpp
 * @brief Implementação dos métodos da classe Pipe.
 */
#include "actors/Pipe.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

Pipe::Pipe(float y, float width, float height, PipeType type, ALLEGRO_BITMAP* texture)
    : GameObject(0, y, width, height), // O x do GameObject base é ignorado.
      pipeType(type), 
      texture(texture) 
{}

void Pipe::draw(float x) const {
    if (!texture) {
        al_draw_filled_rectangle(x, this->y, x + this->width, this->y + this->height, al_map_rgb(0, 255, 0));
        return;
    }
    
    if (pipeType == PipeType::TOP) {
        al_draw_rotated_bitmap(texture, width / 2, height / 2, x + width / 2, y + height / 2, ALLEGRO_PI, 0);
    } 
    else {
        al_draw_bitmap(texture, x, y, 0);
    }
}