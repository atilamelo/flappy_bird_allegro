#include "actors/Bird.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <iostream>
#include "Constants.hpp"
#include <cmath>

Bird::Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP *> frames) : GameObject(x, y, w, h),
                                                                                       frames(frames)
{
    this->velY = 0;
}

void Bird::draw()
{
    if (!frames.empty())
    {
        float radian_angles = angle * (ALLEGRO_PI / 180.0f);
        // O ângulo é negativo porque o Allegro considera o ângulo positivo no sentido horário
        if(!isDying) {
            al_draw_rotated_bitmap(frames[currentFrameIndex], width / 2, height / 2, x + width / 2, y + height / 2, -radian_angles, 0);
        } else {
            al_draw_rotated_bitmap(frames[1], width / 2, height / 2, x + width / 2, y + height / 2, -radian_angles, 0);
        }
    }
    else
    {
        std::cerr << "Erro: vetor com frames vazios" << std::endl;
        al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb_f(255, 0, 0));
    }
}

void Bird::update(float deltaTime)
{
    timeSinceLastFrame += deltaTime;

    if (physicsEnabled)
    {
        std::cout << "Time: " << al_get_time() << "s, Bird Y: " << y << ", velY: " << velY << std::endl;

        velY += GRAVITY * deltaTime;
        // --- Limite de velocidade terminal para a queda do pássaro ---
        if (velY > TERMINAL_VELOCITY)
        {
            velY = TERMINAL_VELOCITY;
        }

        // --- Suavização do movimento vertical ---
        y += velY * deltaTime;


        if (y < 0)
        {
            y = 0;    // Impede que o pássaro saia da tela para cima
            velY = 0; // Reseta a velocidade quando atinge o topo
        }
        

    }

    if (velY < 0) {
        angle = (velY / JUMP_IMPULSE_VELOCITY) * MAX_UP_ANGLE;
    } else {
        angle = (velY / TERMINAL_VELOCITY) * MAX_DOWN_ANGLE;
    }

    if(hoverEnabled) {
        hover(deltaTime);
    }

    // --- Atualização de animação do pássaro ---
    if (timeSinceLastFrame >= frameTime)
    {
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        timeSinceLastFrame = 0.0f;
    }
}

void Bird::jump()
{
    if(!isDying)
    {
        velY = JUMP_IMPULSE_VELOCITY;
    }
}

void Bird::hover(float deltaTime) {
    this->hoverTime += deltaTime;

    const float hoverAmplitude = 10.0f; // Quão "alto" e "baixo" o pássaro vai
    const float hoverSpeed = 4.0f;     // Quão rápido o pássaro flutua

    // 3. A fórmula mágica:
    float y_offset = hoverAmplitude * sin(this->hoverTime * hoverSpeed);

    // 4. Aplicamos o deslocamento à posição Y inicial do pássaro.
    this->y = BIRD_START_Y + y_offset;

    // 5. Garantimos que o pássaro não rotacione durante o hover.
    this->angle = 0;
}

void Bird::die() {
    this->isDying = true;
}
