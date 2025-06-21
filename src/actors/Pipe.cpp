#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "util/ResourceManager.hpp"
#include "actors/Pipe.hpp"
#include "Constants.hpp"

Pipe::Pipe(float x, float y, float width, float height, float speed, PipeType type, ALLEGRO_BITMAP* texture)
    : GameObject(x, y, width, height), speed(speed), pipeType(type), texture(texture) {
    }

void Pipe::draw() {
    if (!texture) {
        al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb_f(0, 255, 0));
        std::cerr << "Erro: textura do pipe não carregada!" << std::endl;
        return;
    }
    
    if(pipeType == PipeType::TOP) {
        al_draw_rotated_bitmap(texture, width / 2, height / 2, x + width / 2, y + height / 2, ALLEGRO_PI, 0);
    } else {
        al_draw_bitmap_region(texture, 0, 0, width, height, x, y, 0);
    }
}

void Pipe::update(float deltaTime) {}

float Pipe::getSpeed() const {
    return speed;
}

void Pipe::setSpeed(float speed) {
    this->speed = speed;
}