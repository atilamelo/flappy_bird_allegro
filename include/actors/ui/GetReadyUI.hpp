/**
 * @file GetReadyUI.hpp
 * @brief Define a UI "Get Ready" que aparece no início de uma partida.
 */
#pragma once

#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <allegro5/allegro.h>

/**
 * @class GetReadyUI
 * @brief Gerencia a exibição e animação dos elementos "Get Ready" e das instruções de toque.
 */
class GetReadyUI : public IDrawable, public IUpdatable {
private:
    bool isVisible;             ///< Controla se a UI deve ser desenhada.
    
    // Recursos visuais
    ALLEGRO_BITMAP* getReadyTexture;
    ALLEGRO_BITMAP* tapInstructionsTexture;

    // Posições calculadas
    float getReadyX, getReadyY;
    float tapX, tapY;

    // Animação de pulsação para as instruções de toque
    float tapAlpha;             ///< Opacidade atual do ícone de toque.
    float pulseTime;            ///< Temporizador para a função de seno.
    const float PULSE_SPEED = 3.0f;

public:
    /**
     * @brief Construtor da GetReadyUI.
     * Carrega os recursos e calcula as posições dos elementos.
     */
    GetReadyUI();

    /**
     * @brief Torna a UI visível.
     */
    void show();

    /**
     * @brief Torna a UI invisível.
     */
    void hide();
    
    // Implementação das interfaces
    void update(float deltaTime) override;
    void draw() const override;
};