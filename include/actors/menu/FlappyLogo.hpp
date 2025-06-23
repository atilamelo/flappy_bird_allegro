/**
 * @file FlappyLogo.hpp
 * @brief Definição da classe FlappyLogo, uma entidade animada para o menu.
 */
#pragma once

#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <vector>
#include <allegro5/allegro.h>

/**
 * @class FlappyLogo
 * @brief Representa a logo animada do jogo, com texto e um pássaro que flutuam juntos.
 */
class FlappyLogo : public GameObject, public IDrawable, public IUpdatable {
private:
    // Constantes de Comportamento
    const float HOVER_AMPLITUDE = 8.0f;
    const float HOVER_SPEED = 4.0f;
    const float FRAME_TIME = 0.1f;

    // Recursos
    ALLEGRO_BITMAP* logoTexture;
    std::vector<ALLEGRO_BITMAP *> birdFrames;

    // Estado
    float startY;               ///< Posição Y inicial, usada como âncora para a flutuação.
    float hoverTime;
    int currentFrameIndex;
    float timeSinceLastFrame;

public:
    FlappyLogo(float x, float y, float w, float h, ALLEGRO_BITMAP* logo, std::vector<ALLEGRO_BITMAP *> frames);

    void draw() const override;
    void update(float deltaTime) override;
};