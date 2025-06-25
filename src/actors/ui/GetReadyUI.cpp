/**
 * @file GetReadyUI.cpp
 * @brief Implementação da classe GetReadyUI.
 */
#include "actors/ui/GetReadyUI.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <cmath> // Para sin()

GetReadyUI::GetReadyUI() : isVisible(false), tapAlpha(1.0f), pulseTime(0.0f) {
    ResourceManager& rm = ResourceManager::getInstance();
    
    // Carrega as texturas através do ResourceManager
    tapInstructionsTexture = rm.getBitmap("getready");

    if(!tapInstructionsTexture) {
        throw std::runtime_error("Falha ao carregar getready");
    }

    if (tapInstructionsTexture) {
        tapX = (BUFFER_W- al_get_bitmap_width(tapInstructionsTexture)) / 2.0f;
        tapY = PLAYABLE_AREA_HEIGHT / 3.0f;
    }
}

void GetReadyUI::show() {
    isVisible = true;
    pulseTime = 0.0f; // Reseta o tempo da animação
}

void GetReadyUI::hide() {
    isVisible = false;
}

void GetReadyUI::update(float deltaTime) {
    if (!isVisible) return;

    // Cria uma animação de pulsação (fade in/out) para o ícone de toque
    pulseTime += deltaTime;
    // A função seno oscila entre -1 e 1. Mapeamos isso para o intervalo de alfa [0, 1]
    tapAlpha = 0.5f + 0.5f * sin(pulseTime * PULSE_SPEED);
}

void GetReadyUI::draw() const {
    if (!isVisible) return;

    al_draw_tinted_scaled_bitmap(tapInstructionsTexture,
        al_map_rgba_f(1, 1, 1, tapAlpha), // Cor com alfa variável
        0, 0, al_get_bitmap_width(tapInstructionsTexture), al_get_bitmap_height(tapInstructionsTexture),
        tapX, tapY,
        al_get_bitmap_width(tapInstructionsTexture), al_get_bitmap_height(tapInstructionsTexture),
        0
    );
    
}