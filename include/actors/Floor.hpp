#pragma once

#include <allegro5/allegro.h>
#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"

/**
 * @file Floor.hpp
 * @brief Define a classe Floor, que representa um objeto de chão renderizável e atualizável no jogo.
 * @author Seu Nome (ou nome do time)
 * @date Data de criação (ex: 25/06/2025)
 */

/**
 * @class Floor
 * @brief Representa o chão do cenário.
 *
 * @details Esta classe herda de GameObject para ter uma posição no mundo do jogo,
 * de IDrawable para ser desenhada na tela a cada quadro, e de IUpdatable para
 * atualizar sua lógica, como o movimento de parallax scrolling.
 */
class Floor : public GameObject, public IDrawable, public IUpdatable
{
private:
    /**
     * @var texture
     * @brief Ponteiro para a textura (bitmap) do chão.
     * @details A classe não gerencia a memória deste bitmap, apenas o utiliza para renderização.
     * O bitmap deve ser carregado e destruído externamente.
     */
    ALLEGRO_BITMAP* texture;

public:
    /**
     * @brief Construtor da classe Floor.
     * @param texture Um ponteiro para o bitmap do Allegro que será usado como textura do chão.
     */
    Floor(ALLEGRO_BITMAP* texture);

    /**
     * @brief Desenha o chão na tela.
     * @details Este método é uma sobrescrita de IDrawable::draw(). É chamado pelo
     * loop de renderização do jogo para desenhar a textura do chão na posição atual do objeto.
     * @note A implementação deve estar no arquivo .cpp correspondente.
     */
    void draw() const override;

    /**
     * @brief Atualiza a lógica do chão.
     * @details Este método é uma sobrescrita de IUpdatable::update(). É chamado a cada quadro
     * pelo loop principal do jogo. Pode ser usado para implementar efeitos como o parallax scrolling,
     * atualizando a posição do chão.
     * @param deltaTime O tempo, em segundos, decorrido desde o último quadro. Essencial para
     * manter a velocidade do movimento consistente independentemente do framerate.
     * @note A implementação deve estar no arquivo .cpp correspondente.
     */
    void update(float deltaTime) override;
};