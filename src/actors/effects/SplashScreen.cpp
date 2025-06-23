/**
 * @file SplashScreen.cpp
 * @brief Implementação da classe SplashScreen.
 */

#include "actors/effects/SplashScreen.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>

SplashScreen::SplashScreen(float duration, ALLEGRO_COLOR flashColor)
    // O GameObject em si pode ter tamanho 0, pois usamos o buffer para desenhar
    : GameObject(0, 0, 0, 0),
      isActive(false),
      currentAlpha(0.0f),
      fadeDuration(duration),
      elapsedTime(0.0f),
      color(flashColor)
{}

void SplashScreen::trigger() {
    isActive = true;
    currentAlpha = 1.0f; // Começa totalmente opaco
    elapsedTime = 0.0f;
}

void SplashScreen::update(float deltaTime) {
    if (!isActive) return;

    elapsedTime += deltaTime;

    // Interpolação linear para diminuir a opacidade
    currentAlpha = 1.0f - (elapsedTime / fadeDuration);

    // Garante que o alfa não se torne negativo e desativa o efeito
    if (currentAlpha <= 0.0f) {
        currentAlpha = 0.0f;
        isActive = false;
    }
}

void SplashScreen::draw() const {
    if (!isActive) return;

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    ALLEGRO_COLOR a_color = color;
    a_color.a = currentAlpha;

    // Desenha um retângulo que preenche a tela inteira
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, a_color);
}