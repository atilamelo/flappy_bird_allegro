/**
 * @file FlappyLogo.cpp
 * @brief Implementação da classe FlappyLogo.
 */
#include "actors/menu/FlappyLogo.hpp"
#include "Constants.hpp"
#include <cmath>

FlappyLogo::FlappyLogo(float x, float y, float w, float h, ALLEGRO_BITMAP *logo, std::vector<ALLEGRO_BITMAP *> frames) 
    : GameObject(x, y, w, h),
      logoTexture(logo),
      birdFrames(frames)
{
    // Armazena a posição Y inicial para ser o centro da flutuação.
    this->startY = y;

    // Inicializa os outros estados
    this->hoverTime = 0.0f;
    this->currentFrameIndex = 0;
    this->timeSinceLastFrame = 0.0f;
}

void FlappyLogo::update(float deltaTime)
{
    // --- Lógica de Flutuação ---
    // A flutuação agora atualiza a posição Y principal do objeto GameObject.
    hoverTime += deltaTime;
    float y_offset = HOVER_AMPLITUDE * sin(hoverTime * HOVER_SPEED);
    this->y = this->startY + y_offset; // O objeto inteiro se move para cima e para baixo.

    // --- Lógica de Animação das Asas ---
    timeSinceLastFrame += deltaTime;
    if (timeSinceLastFrame >= FRAME_TIME) {
        currentFrameIndex = (currentFrameIndex + 1) % birdFrames.size();
        timeSinceLastFrame = 0.0f;
    }
}

void FlappyLogo::draw() const
{
    // --- 1. Desenha o logo de texto "Flappy Bird" ---
    // Ele é desenhado na posição (x, y) flutuante do objeto.
    if (logoTexture) {
        al_draw_scaled_bitmap(logoTexture,
                              0, 0, al_get_bitmap_width(logoTexture), al_get_bitmap_height(logoTexture),
                              this->x, this->y,
                              this->width, this->height, 0);
    }

    // --- 2. Desenha o pássaro animado ---
    // Ele também usa a posição flutuante como base, mas com um deslocamento para a direita.
    if (!birdFrames.empty()) {
        const float spacing = 10.0f; // Espaço entre o logo e o pássaro
        
        // Posição do pássaro: ao lado direito e centralizado verticalmente com o logo.
        float bird_x = this->x + this->width + spacing;
        float bird_y = this->y + (this->height / 2) - (BIRD_HEIGHT / 2);

        al_draw_bitmap(birdFrames[currentFrameIndex], bird_x, bird_y, 0);
    }
}