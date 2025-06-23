/**
 * @file GameOverScreen.cpp
 * @brief Implementação da classe GameOverScreen.
 */
#include "actors/ui/GameOverScreen.hpp"
#include "Constants.hpp"
#include <cmath> // Para sin() na função de easing

GameOverScreen::GameOverScreen(float finalY, float duration, ALLEGRO_BITMAP* gameOverTexture)
    // Inicializa a largura e altura com base na textura, e a posição X já centralizada.
    : GameObject((BUFFER_W - al_get_bitmap_width(gameOverTexture)) / 2.0f, 0, al_get_bitmap_width(gameOverTexture), al_get_bitmap_height(gameOverTexture)),
      isActive(false),
      elapsedTime(0.0f),
      animationDuration(duration),
      endY(finalY),
      texture(gameOverTexture)
{
    // A posição inicial é acima da tela para o efeito de "cair"
    this->startY = -this->height;
    this->y = this->startY;
}

void GameOverScreen::startAnimation() {
    // Reseta o estado para garantir que a animação comece do início
    this->elapsedTime = 0.0f;
    this->y = this->startY;
    this->isActive = true;
}

void GameOverScreen::reset() {
    this->isActive = false;
    this->elapsedTime = 0.0f;
    this->y = this->startY;
}

void GameOverScreen::update(float deltaTime) {
    if (!isActive || elapsedTime >= animationDuration) return;

    elapsedTime += deltaTime;
    
    // Garante que o tempo não ultrapasse a duração
    float progress = elapsedTime / animationDuration;
    if (progress > 1.0f) progress = 1.0f;

    // --- Animação Suave com seno ---
    float easedProgress = sin(progress * (ALLEGRO_PI / 2.0));

    // Interpola a posição Y com base no progresso suavizado
    this->y = startY + (endY - startY) * easedProgress;
}

void GameOverScreen::draw() const {
    if (!isActive || !texture) return;
    al_draw_bitmap(texture, x, y, 0);
}