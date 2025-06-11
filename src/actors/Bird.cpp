#include "actors/Bird.hpp"

#include <allegro5/allegro_primitives.h>
#include <iostream>

#define GRAVITY 500.0f
#define JUMP_FORCE 250.0f
#define TERMINAL_VELOCITY 300.0f

Bird::Bird(float x, float y, float w, float h,  float vy) : GameObject(x, y, w, h) {
    this->velocityY = vy;
}

void Bird::draw(float deltaTime) {
    al_draw_circle(x, y, width, al_map_rgb_f(255, 255, 0), 2);
}

void Bird::update(float deltaTime) {
    velocityY += GRAVITY * deltaTime;

    // Velocidade Terminal
    if (velocityY > TERMINAL_VELOCITY) {
        velocityY = TERMINAL_VELOCITY;
    }

    // Delta Time é o tempo entre atualizações, usado para suavizar o movimento
    y += velocityY * deltaTime;
}

void Bird::jump() {
    velocityY = -JUMP_FORCE; 
}