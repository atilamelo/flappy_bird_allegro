/**
 * @file Bird.hpp
 * @brief Definição da classe Bird, que representa o pássaro controlado pelo jogador.
 * @version 2.0
 */
#pragma once

#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <vector>
#include <allegro5/allegro.h>

/**
 * @class Bird
 * @brief Gerencia o estado, a física e a renderização do pássaro do jogo.
 *
 * Esta classe é responsável por todos os comportamentos do pássaro, incluindo
 * seu movimento de queda (física), pulo, animação de bater de asas, flutuação
 * na tela inicial e a sequência de morte.
 */
class Bird : public GameObject, public IDrawable, public IUpdatable
{
private:
    // --- Constantes de Comportamento ---
    const float HOVER_AMPLITUDE = 8.0f;  ///< Amplitude da flutuação em pixels.
    const float HOVER_SPEED = 4.0f;      ///< Velocidade da flutuação.
    const float DEATH_ROTATION_SPEED = 4.0f; ///< Velocidade de rotação ao morrer.

    // --- Estado da Física e Movimento ---
    float velY;                         ///< Velocidade vertical atual.
    float angle;                        ///< Ângulo de rotação atual em graus.
    float hoverTime;                    ///< Temporizador para o cálculo do efeito de flutuação.

    // --- Estado da Animação ---
    std::vector<ALLEGRO_BITMAP *> frames; ///< Vetor de bitmaps para a animação.
    int currentFrameIndex;              ///< Índice do frame de animação atual.
    float frameTime;                    ///< Duração de cada frame de animação.
    float timeSinceLastFrame;           ///< Tempo acumulado desde a última troca de frame.

    // --- Flags de Estado Interno ---
    bool physicsEnabled;                ///< Flag que ativa a física (gravidade e movimento).
    bool hoverEnabled;                  ///< Flag que ativa o efeito de flutuação.
    bool isDying;                       ///< Flag que indica se o pássaro está na sequência de morte.

    /**
     * @brief Executa a lógica de flutuação suave na tela inicial.
     * @param deltaTime O tempo decorrido desde o último frame.
     */
    void applyHover(float deltaTime);

public:
    /**
     * @brief Construtor da classe Bird.
     * @param x Posição inicial X.
     * @param y Posição inicial Y.
     * @param w Largura do pássaro.
     * @param h Altura do pássaro.
     * @param frames Vetor de ponteiros para os bitmaps da animação.
     */
    Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP *> frames);

    /**
     * @brief Atualiza o estado do pássaro a cada frame.
     *
     * Este método gerencia qual lógica aplicar (física, flutuação, morte)
     * com base nas flags de estado internas.
     * @param deltaTime O tempo decorrido desde o último frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Desenha o pássaro na tela.
     */
    void draw() const override;

    /**
     * @brief Aplica um impulso vertical para fazer o pássaro pular.
     */
    void jump();

    /**
     * @brief Inicia a sequência de morte do pássaro.
     */
    void die();

    /**
     * @brief Restaura o pássaro ao seu estado inicial.
     *
     * Usado para reiniciar o jogo sem precisar recriar o objeto.
     */
    void reset();

    // --- Setters para controle externo (pela GameScene) ---

    /**
     * @brief Ativa ou desativa a física do pássaro.
     * @param enable true para ativar, false para desativar.
     */
    void setPhysicsEnabled(bool enable) { this->physicsEnabled = enable; }

    /**
     * @brief Ativa ou desativa o efeito de flutuação do pássaro.
     * @param enable true para ativar, false para desativar.
     */
    void setHoverEnabled(bool enable) { this->hoverEnabled = enable; }
};