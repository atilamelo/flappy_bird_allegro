/**
 * @file Pipe.hpp
 * @brief Definição da classe Pipe, um componente visual de um PipePair.
 */
#pragma once

#include "core/GameObject.hpp" // Herda para obter y, width, height
#include <allegro5/allegro.h>

/**
 * @enum PipeType
 * @brief Enumera os tipos de cano para diferenciação na renderização.
 */
enum class PipeType {
    TOP,
    BOTTOM
};

/**
 * @class Pipe
 * @brief Componente visual que representa uma metade de um par de canos.
 *
 * Esta classe é intencionalmente simples. Ela não gerencia sua própria posição
 * horizontal ou lógica de movimento. Sua única responsabilidade é saber como
 * se desenhar em uma coordenada X que é fornecida externamente.
 */
class Pipe : public GameObject {
private:
    ALLEGRO_BITMAP* texture;
    PipeType pipeType;

public:
    /**
     * @brief Construtor da classe Pipe.
     * @param y Posição Y na tela.
     * @param width A largura do cano.
     * @param height A altura do cano.
     * @param type O tipo do cano (TOP ou BOTTOM).
     * @param texture Ponteiro para a textura do cano.
     */
    Pipe(float y, float width, float height, PipeType type, ALLEGRO_BITMAP* texture);
    
    /**
     * @brief Desenha o cano na tela na posição X fornecida.
     * @note Este método NÃO é uma implementação da interface IDrawable, pois sua assinatura é diferente.
     * @param x A coordenada X onde o cano deve ser desenhado.
     */
    void draw(float x) const;
};