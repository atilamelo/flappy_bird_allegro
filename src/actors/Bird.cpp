#include "actors/Bird.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "Constants.hpp"

Bird::Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP*> frames) :
    GameObject(x, y, w, h),
    frames(frames)
{
    this->velocityY = GRAVITY;
}

void Bird::draw() {
    if (!frames.empty()) {
        al_draw_bitmap(frames[currentFrameIndex], x, y, 0);
    } else {
        std::cerr << "Erro: vetor com frames vazios" << std::endl;
        al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb_f(255, 0, 0));
    }
}

void Bird::update(float deltaTime) {
    timeSinceLastFrame += deltaTime;
    velocityY += GRAVITY * deltaTime;

    if(timeSinceLastFrame >= frameTime) {
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        timeSinceLastFrame = 0.0f;
    }

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