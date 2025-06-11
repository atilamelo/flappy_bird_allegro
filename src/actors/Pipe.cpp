#include <allegro5/allegro_primitives.h>
#include "actors/Pipe.hpp"
#include "Constants.hpp"

Pipe::Pipe(float x, float y, float width, float height, float speed)
    : GameObject(x, y, width, height), speed(speed) {}


void Pipe::draw(float deltaTime) {
    al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb_f(0, 255, 0));
}

void Pipe::update(float deltaTime) {
    x -= speed * deltaTime;
}

float Pipe::getSpeed() const {
    return speed;
}

void Pipe::setSpeed(float speed) {
    this->speed = speed;
}