#include "actors/Bird.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <iostream>
#include "Constants.hpp"

Bird::Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP*> frames) :
    GameObject(x, y, w, h),
    frames(frames)
{
    this->velY = 0;
}

void Bird::draw() {
    if (!frames.empty()) {
        float radian_angles = angle * (ALLEGRO_PI / 180.0f);
        // O ângulo é negativo porque o Allegro considera o ângulo positivo no sentido horário
        al_draw_rotated_bitmap(frames[currentFrameIndex], width / 2, height / 2, x + width / 2, y + height / 2, -radian_angles, 0);
    } else {
        std::cerr << "Erro: vetor com frames vazios" << std::endl;
        al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb_f(255, 0, 0));
    }
}

void Bird::update(float deltaTime) {
    timeSinceLastFrame += deltaTime;
    velY += GRAVITY * deltaTime;
    // --- Limite de velocidade terminal para a queda do pássaro ---
    if (velY > TERMINAL_VELOCITY) {
        velY = TERMINAL_VELOCITY;
    }

    // --- Suavização do movimento vertical ---
    y += velY * deltaTime;
    
    if(y < 0) {
        y = 0; // Impede que o pássaro saia da tela para cima
        velY = 0; // Reseta a velocidade quando atinge o topo
    }
    if(y > BUFFER_H - height) {
        y = BUFFER_H - height; // Impede que o pássaro saia da tela para baixo
        velY = 0; // Reseta a velocidade quando atinge o fundo
    }

    // --- Cálculo do ângulo baseado na velocidade Y ---
    if (velY < 0) {
        angle = (velY / (JUMP_IMPULSE_VELOCITY) * MAX_UP_ANGLE);  
    } else {
        angle = (velY / TERMINAL_VELOCITY) * MAX_DOWN_ANGLE;
    }

    // --- Atualização de animação do pássaro ---
    if(timeSinceLastFrame >= frameTime) {
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        timeSinceLastFrame = 0.0f;
    }
}

void Bird::jump() {
    velY = JUMP_IMPULSE_VELOCITY;
}