#include "actors/Bird.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "Constants.hpp"

Bird::Bird(float x, float y, float w, float h) : GameObject(x, y, w, h) {
    this->velocityY = GRAVITY;
}

void Bird::draw() {
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
    if(y < 0) {
        y = 0; // Impede que o pássaro saia da tela para cima
        velocityY = 0; // Reseta a velocidade quando atinge o topo
    }
    if(y > BUFFER_H - height) {
        y = BUFFER_H - height; // Impede que o pássaro saia da tela para baixo
        velocityY = 0; // Reseta a velocidade quando atinge o fundo
    }
}

void Bird::jump() {
    velocityY = -JUMP_FORCE; 
}