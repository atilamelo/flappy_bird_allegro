#include <allegro5/allegro_primitives.h>
#include "util/ResourceManager.hpp"
#include "actors/Pipe.hpp"
#include "Constants.hpp"

Pipe::Pipe(float x, float y, float width, float height, float speed, PipeType type)
    : GameObject(x, y, width, height), speed(speed), pipeType(type) {
        texture = ResourceManager::getInstance().getBitmap("pipe-green");
        if (!texture) {
            throw std::runtime_error("Falha ao carregar textura do tubo");
        }
    }

void Pipe::draw() {
    if(!texture) {
        throw std::runtime_error("Tubo não tem textura carregada");
    }
    
    if(pipeType == PipeType::TOP) {
        al_draw_rotated_bitmap(texture, width / 2, height / 2, x + width / 2, y + height / 2, ALLEGRO_PI, 0);
    } else {
        al_draw_bitmap_region(texture, 0, 0, width, height, x, y, 0);
    }
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