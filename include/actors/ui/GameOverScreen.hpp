/**
 * @file GameOverScreen.hpp
 * @brief Define a tela de "Game Over" com animação de entrada.
 */
#pragma once

#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <allegro5/allegro.h>

/**
 * @class GameOverScreen
 * @brief Um GameObject que exibe a imagem de "Game Over" e a anima
 * deslizando de cima para sua posição final.
 */
class GameOverScreen : public GameObject, public IDrawable, public IUpdatable {
private:
    bool isActive;              ///< Controla se o objeto está ativo (visível e animando).
    float elapsedTime;          ///< Tempo decorrido desde o início da animação.
    float animationDuration;    ///< Duração total da animação de deslize.
    float startY;               ///< Posição Y inicial (acima da tela).
    float endY;                 ///< Posição Y final (onde a imagem irá parar).
    ALLEGRO_BITMAP* texture;    ///< A textura "Game Over".

public:
    /**
     * @brief Construtor da GameOverScreen.
     * @param finalY A posição Y final onde a imagem deve parar.
     * @param duration A duração da animação de deslize.
     * @param gameOverTexture A imagem a ser exibida.
     */
    GameOverScreen(float finalY, float duration, ALLEGRO_BITMAP* gameOverTexture);

    /**
     * @brief Inicia a animação de deslize.
     */
    void startAnimation();

    /**
     * @brief Reseta o estado do objeto para invisível/inativo.
     */
    void reset();

    // --- Implementação das Interfaces ---
    void update(float deltaTime) override;
    void draw() const override;
};