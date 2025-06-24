/**
 * @file GameOverScreen.hpp
 * @brief Define a UI completa de Fim de Jogo, com animações em múltiplos estágios e escala no placar.
 * @version 2.1
 * @date 2025-06-23
 */
#pragma once

#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include "managers/ScoreManager.hpp"
#include <allegro5/allegro.h>

/**
 * @class GameOverScreen
 * @brief Gerencia a exibição e animação de toda a interface de fim de jogo,
 * incluindo o bitmap "Game Over", o painel de placar (escalado) e a medalha.
 */
class GameOverScreen : public IDrawable, public IUpdatable {
private:
    // Constante para o fator de escala do placar.
    static float constexpr SCOREBOARD_SCALE = 2.0f;

    // Enum para controlar os estágios da animação interna.
    enum class AnimationState {
        INACTIVE,
        SLIDING_GAMEOVER_BITMAP,
        SLIDING_SCOREBOARD,
        FINISHED
    };

    AnimationState currentState;

    // --- Recursos Visuais ---
    ALLEGRO_BITMAP* gameOverTexture; // Bitmap para o título "Game Over"
    ALLEGRO_BITMAP* boardTexture;    // Bitmap para o painel de score
    ALLEGRO_BITMAP* currentMedal;

    // --- Objetos e Referências ---
    const ScoreManager& scoreManagerRef;

    // --- Posições e Animação ---
    GameObject gameOverGO; // GameObject para o título "Game Over"
    float gameOver_startY;
    float gameOver_endY;

    GameObject scoreBoardGO; // GameObject para o painel de score
    float scoreBoard_startY;
    float scoreBoard_endY;

    float elapsedTime;
    float animationDuration;

    // --- Dados do Jogo ---
    int finalScore;
    int bestScore;

    /**
     * @brief Define qual medalha deve ser exibida com base na pontuação.
     */
    void determineMedal();

public:
    /**
     * @brief Construtor da GameOverScreen.
     * @param scManager Referência ao ScoreManager para poder desenhar os números.
     */
    GameOverScreen(const ScoreManager& scManager);

    /**
     * @brief Inicia a sequência completa de animação de fim de jogo.
     * @param score A pontuação final da partida.
     * @param best A melhor pontuação registrada.
     */
    void startSequence(int score, int best);

    /**
     * @brief Reseta a UI para seu estado inicial (invisível).
     */
    void reset();

    // Implementação das interfaces
    void update(float deltaTime) override;
    void draw() const override;
};