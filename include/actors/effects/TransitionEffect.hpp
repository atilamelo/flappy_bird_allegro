/**
 * @file TransitionEffect.hpp
 * @brief Define um efeito de transição de tela (fade-in/fade-out).
 */
#pragma once

#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <allegro5/allegro.h>

/**
 * @enum FadeState
 * @brief Os possíveis estados do efeito de transição.
 */
enum class FadeState {
    IDLE,       // Inativo, invisível
    FADING_OUT, // Escurecendo a tela (alfa de 0 para 1)
    FADING_IN   // Clareando a tela (alfa de 1 para 0)
};

/**
 * @class TransitionEffect
 * @brief Gerencia uma sobreposição de cor que pode fazer fade-in ou fade-out.
 */
class TransitionEffect : public IDrawable, public IUpdatable {
private:
    FadeState state;
    float currentAlpha;
    float duration;
    float elapsedTime;
    ALLEGRO_COLOR color;

public:
    /**
     * @brief Construtor do efeito de transição.
     * @param fadeColor A cor para a qual a tela irá fazer o fade.
     * @param fadeDuration A duração de cada fade (in ou out).
     */
    TransitionEffect(ALLEGRO_COLOR fadeColor, float fadeDuration);

    void fadeOut(); // Inicia o fade-out
    void fadeIn();  // Inicia o fade-in

    bool isFinished() const { return state == FadeState::IDLE; }

    // Implementação das interfaces
    void update(float deltaTime) override;
    void draw() const override;
};