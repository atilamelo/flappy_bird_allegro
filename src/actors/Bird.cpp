/**
 * @file Bird.cpp
 * @brief Implementação dos métodos da classe Bird.
 */

#include "actors/Bird.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <cmath>
#include "Constants.hpp"

Bird::Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP *> frames) : GameObject(x, y, w, h),
                                                                                       frames(frames)
{
    // Chama o método reset para garantir um estado inicial limpo e centralizado.
    reset();
}

void Bird::draw()
{
    if (frames.empty()) {
        std::cerr << "Erro: Vetor de frames do pássaro está vazio!" << std::endl;
        al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb(255, 0, 0));
        return;
    }

    float radian_angles = angle * (ALLEGRO_PI / 180.0f);
    int frameToDraw = isDying ? 1 : currentFrameIndex; // Se estiver morrendo, trava no frame 1 (asas paradas)

    al_draw_rotated_bitmap(frames[frameToDraw], width / 2, height / 2, x + width / 2, y + height / 2, -radian_angles, 0);
}

void Bird::update(float deltaTime)
{
    // 1. LÓGICA DE MOVIMENTO E FÍSICA
    if (isDying) {
        // Se está morrendo, a física continua, mas o ângulo é forçado para baixo.
        velY += GRAVITY * deltaTime;
        y += velY * deltaTime;
        
        // Gira o pássaro gradualmente até atingir o ângulo máximo de queda.
        if (angle < MAX_DOWN_ANGLE) {
            angle += DEATH_ROTATION_SPEED;
            if (angle > MAX_DOWN_ANGLE) angle = MAX_DOWN_ANGLE;
        }
    }
    else if (hoverEnabled) {
        // Se a flutuação está ativa, aplica o efeito de seno.
        applyHover(deltaTime);
    }
    else if (physicsEnabled) {
        // Se a física normal está ativa (durante o jogo).
        velY += GRAVITY * deltaTime;
        if (velY > TERMINAL_VELOCITY) {
            velY = TERMINAL_VELOCITY;
        }
        y += velY * deltaTime;
        
        // O cálculo do ângulo só acontece quando a física está ativa.
        if (velY < JUMP_IMPULSE_VELOCITY / 2) {
            angle = (velY / (JUMP_IMPULSE_VELOCITY) * MAX_UP_ANGLE);
        } else {
            angle = (velY / TERMINAL_VELOCITY) * MAX_DOWN_ANGLE;
        }
    }
    
    // 2. LÓGICA DE ANIMAÇÃO
    // A animação das asas só acontece se o pássaro não estiver morrendo.
    if (!isDying) {
        timeSinceLastFrame += deltaTime;
        if (timeSinceLastFrame >= frameTime) {
            currentFrameIndex = (currentFrameIndex + 1) % frames.size();
            timeSinceLastFrame = 0.0f;
        }
    }
}

void Bird::jump()
{
    // O pulo só funciona se a física estiver ativa e o pássaro não estiver morrendo.
    if (physicsEnabled && !isDying)
    {
        velY = JUMP_IMPULSE_VELOCITY;
    }
}

void Bird::die()
{
    if (!isDying) {
        isDying = true;
        velY = JUMP_IMPULSE_VELOCITY * 0.5f;
    }
}

void Bird::reset()
{
    // Restaura todas as variáveis para seus valores iniciais.
    x = BIRD_START_X;
    y = BIRD_START_Y;
    velY = 0.0f;
    angle = 0.0f;
    
    currentFrameIndex = 0;
    timeSinceLastFrame = 0.0f;
    hoverTime = 0.0f;
    frameTime = 0.1f;

    // Restaura as flags para o estado de início de jogo (flutuando).
    isDying = false;
    physicsEnabled = false;
    hoverEnabled = true;
}

void Bird::applyHover(float deltaTime)
{
    hoverTime += deltaTime;
    
    // Usa a função seno para criar uma oscilação entre -HOVER_AMPLITUDE e +HOVER_AMPLITUDE
    float y_offset = HOVER_AMPLITUDE * sin(hoverTime * HOVER_SPEED);

    // Aplica o deslocamento à posição Y inicial do pássaro.
    this->y = BIRD_START_Y + y_offset;
    this->angle = 0; // Garante que o pássaro fique reto durante a flutuação.
}