/**
 * @file ParallaxBackground.hpp
 * @brief Definição da classe ParallaxBackground para criar um fundo com rolagem infinita.
 */
#pragma once

#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <allegro5/allegro.h>
#include "Constants.hpp"

/**
 * @class ParallaxBackground
 * @brief Gerencia um objeto de fundo que se move continuamente para criar um efeito de parallax.
 *
 * Esta classe desenha uma imagem de fundo duas vezes, lado a lado, e a move
 * para a esquerda. Quando a primeira imagem sai completamente da tela, ela é
 * reposicionada à direita da segunda, criando uma ilusão de rolagem infinita.
 */
class ParallaxBackground : public GameObject, public IDrawable, public IUpdatable
{
private:
    ALLEGRO_BITMAP* texture; ///< Ponteiro para a textura do fundo.
    float speed;             ///< Velocidade de rolagem horizontal em pixels por segundo.

public:
    /**
     * @brief Construtor da classe ParallaxBackground.
     * @param image Ponteiro para o bitmap a ser usado como fundo.
     * @param scrollSpeed A velocidade na qual o fundo deve rolar.
     */
    ParallaxBackground(ALLEGRO_BITMAP* image, float scrollSpeed);

    /**
     * @brief Atualiza a posição do fundo a cada frame.
     * @param deltaTime O tempo decorrido desde o último frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Desenha o fundo na tela.
     */
    void draw() const override;

    /**
     * @brief Define uma nova velocidade de rolagem para o fundo.
     * @param newSpeed A nova velocidade.
     */
    void setSpeed(float newSpeed) {
        this->speed = newSpeed;
    }
};