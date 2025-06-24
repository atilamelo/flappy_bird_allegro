/**
 * @file PipePair.hpp
 * @brief Definição da classe PipePair, que gerencia um par de canos do jogo.
 */
#pragma once

#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include "actors/Pipe.hpp"

// Forward declaration para evitar dependência circular se Bird incluir PipePair
class Bird; 

/**
 * @class PipePair
 * @brief Gerencia a lógica, estado e renderização de um par de canos (superior e inferior).
 *
 * Esta classe é a entidade "inteligente" que se move, detecta colisões e
 * contém dois objetos Pipe "burros" que são apenas componentes visuais.
 */
class PipePair : public GameObject, public IDrawable, public IUpdatable {
private:
    bool active;
    bool passed;
    float speed;
    float gap;
    Pipe topPipe;
    Pipe bottomPipe;

public:
    /**
     * @brief Construtor do PipePair.
     */
    PipePair();

    /**
     * @brief Configura o par de canos para uma nova passagem.
     * @param startX Posição X inicial.
     * @param startYGap Posição Y do início do vão.
     * @param gapSize Tamanho do vão.
     * @param scrollSpeed Velocidade de movimento.
     * @param pipeTexture Textura do cano
     */
    void init(float startX, float startYGap, float gapSize, float scrollSpeed, ALLEGRO_BITMAP* pipeTexture);

    /**
     * @brief Reseta o par de canos para o estado inativo.
     */
    void reset();

    // --- Cumprindo os Contratos das Interfaces ---
    
    /**
     * @brief Atualiza a posição do par de canos. (Contrato de IUpdatable)
     * @param deltaTime O tempo desde o último frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Desenha o par de canos na tela. (Contrato de IDrawable)
     */
    void draw() const override;
    
    // --- Lógica de Jogo ---

    bool isColliding(const Bird& bird) const;
    bool hasPassed(const Bird& bird);

    // --- Getters ---
    bool isActive() const { return active; }
    const Pipe& getTopPipe() const { return topPipe; }
    const Pipe& getBottomPipe() const { return bottomPipe; }
};