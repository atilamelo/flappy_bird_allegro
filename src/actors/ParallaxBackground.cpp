/**
 * @file ParallaxBackground.cpp
 * @brief Implementação dos métodos da classe ParallaxBackground.
 */
#include "actors/ParallaxBackground.hpp"

ParallaxBackground::ParallaxBackground(ALLEGRO_BITMAP* image, float scrollSpeed)
    : GameObject(0, 0, 0, 0),
      texture(image),
      speed(scrollSpeed) 
{
    // As dimensões do objeto são herdadas da própria imagem
    if (texture) {
        this->width = al_get_bitmap_width(texture);
        this->height = al_get_bitmap_height(texture);
    }
}

void ParallaxBackground::update(float deltaTime)
{
    // Move a coordenada X para a esquerda com base na velocidade e no tempo.
    x -= speed * deltaTime;


    if (x <= -width)
    {
        x += width;
    }
}

void ParallaxBackground::draw() const
{
    if (!texture) return;

    // Desenha a primeira instância da imagem na posição atual de 'x'.
    al_draw_bitmap(texture, x, y, 0);
    
    // Desenha uma segunda instância da imagem exatamente à direita da primeira.
    // Isso cria a ilusão de um fundo contínuo enquanto 'x' se move.
    al_draw_bitmap(texture, x + width, y, 0);
}