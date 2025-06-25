#include "actors/Floor.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

/**
 * @brief Construtor da classe Floor.
 * @details Inicializa o GameObject base com as dimensões e posição do chão definidas em Constants.hpp.
 * Armazena a textura fornecida e lança uma exceção se a textura for nula.
 * @param texture Ponteiro para o bitmap do Allegro que será usado como textura do chão.
 * @throw std::runtime_error Se o ponteiro da textura for nulo.
 */
Floor::Floor(ALLEGRO_BITMAP *texture) : GameObject(0, PLAYABLE_AREA_HEIGHT, BUFFER_W, FLOOR_HEIGHT)
{
    if (!texture) {
        // Garante que o objeto não seja criado com uma textura inválida.
        throw std::runtime_error("Floor texture is null: failed to initialize Floor object.");
    }
    this->texture = texture;
}

/**
 * @brief Desenha o chão na tela.
 * @details Implementa um efeito de scroll contínuo. Desenha a textura principal na posição (x, y)
 * e, assim que ela começa a sair da tela pela esquerda, desenha uma segunda cópia logo à sua direita
 * para preencher o espaço vazio, criando a ilusão de um chão infinito.
 */
void Floor::draw() const {
    // Desenha a primeira instância da textura do chão.
    al_draw_bitmap(texture, x, y, 0);

    // Se a primeira instância já começou a sair pela esquerda da tela (x < 0),
    // desenha uma segunda instância logo em seguida para criar o loop visual.
    if(x < 0){
        al_draw_bitmap(texture, x + width, y, 0);
    }
}

/**
 * @brief Atualiza a posição horizontal do chão para simular movimento.
 * @param deltaTime O tempo decorrido, em segundos, desde o último quadro.
 */
void Floor::update(float deltaTime) {
    // Move o chão para a esquerda. A velocidade é um pouco maior que a do fundo
    // para dar um efeito de parallax.
    x -= (BACKGROUND_SCROLL_SPEED + 60.0f) * deltaTime;

    // Quando a primeira instância da textura sai completamente da tela pela esquerda,
    // a posição `x` é resetada para 0. Como a segunda instância estava em `x + width`,
    // ela agora estará exatamente em `x=0`, garantindo a continuidade do movimento sem saltos.
    if (x + width < 0) { 
        x = 0;
    }
}
