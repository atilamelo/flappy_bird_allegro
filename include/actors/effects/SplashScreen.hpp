/**
 * @file SplashScreen.hpp
 * @brief Define um efeito de "flash" na tela que desaparece gradualmente.
 */
#pragma once

#include <allegro5/allegro.h>
#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"

/**
 * @class SplashScreen
 * @brief Um GameObject que preenche a tela com uma cor e desaparece ao longo do tempo.
 */
class SplashScreen : public GameObject, public IDrawable, public IUpdatable {
private:
    bool isActive;          ///< Controla se o efeito está ativo.
    float currentAlpha;     ///< A opacidade atual (1.0 = opaco, 0.0 = transparente).
    float fadeDuration;     ///< O tempo total, em segundos, para o efeito desaparecer.
    float elapsedTime;      ///< O tempo decorrido desde que o efeito foi ativado.
    ALLEGRO_COLOR color;    ///< A cor do flash.

public:
    /**
     * @brief Construtor do SplashScreen.
     * @param duration A duração, em segundos, do efeito de fade-out.
     * @param flashColor A cor do flash.
     */
    SplashScreen(float duration, ALLEGRO_COLOR flashColor);

    /**
     * @brief Ativa o efeito de flash.
     */
    void trigger();

    /**
     * @brief Verifica se o efeito já terminou.
     * @return true se o efeito não está mais ativo, false caso contrário.
     */
    bool isFinished() const { return !isActive; }

    // --- Implementação das Interfaces ---
    void update(float deltaTime) override;
    void draw() const override;
};