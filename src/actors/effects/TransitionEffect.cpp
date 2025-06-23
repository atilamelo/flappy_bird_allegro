/**
 * @file TransitionEffect.cpp
 * @brief Implementação da classe TransitionEffect.
 */
#include "actors/effects/TransitionEffect.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>

TransitionEffect::TransitionEffect(ALLEGRO_COLOR fadeColor, float fadeDuration)
    : state(FadeState::IDLE),
      currentAlpha(0.0f),
      duration(fadeDuration),
      elapsedTime(0.0f),
      color(fadeColor)
{}

void TransitionEffect::fadeOut() {
    state = FadeState::FADING_OUT;
    elapsedTime = 0.0f;
}

void TransitionEffect::fadeIn() {
    state = FadeState::FADING_IN;
    elapsedTime = 0.0f;
}

void TransitionEffect::update(float deltaTime) {
    if (state == FadeState::IDLE) return;

    elapsedTime += deltaTime;
    float progress = elapsedTime / duration;

    if (state == FadeState::FADING_OUT) {
        currentAlpha = progress;
        if (currentAlpha >= 1.0f) {
            currentAlpha = 1.0f;
            state = FadeState::IDLE;
        }
    } else if (state == FadeState::FADING_IN) {
        currentAlpha = 1.0f - progress;
        if (currentAlpha <= 0.0f) {
            currentAlpha = 0.0f;
            state = FadeState::IDLE;
        }
    }
}

void TransitionEffect::draw() const {
    if (currentAlpha == 0.0f) return;
    
    ALLEGRO_COLOR a_color = color;
    a_color.a = currentAlpha;
    
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, a_color);
}