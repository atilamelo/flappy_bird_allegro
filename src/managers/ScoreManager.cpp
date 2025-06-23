#include "managers/ScoreManager.hpp"
#include <iostream>
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"

ScoreManager::ScoreManager() {
    for (int i = 0; i < 10; ++i) {
        std::string digitId = std::to_string(i);
        digitSprites[i] = ResourceManager::getInstance().getBitmap(digitId);
        if (!digitSprites[i]) {
            std::cerr << "AVISO: Falha ao carregar sprite para o dígito " << digitId << std::endl;
            throw std::runtime_error("Houve um erro ao carregar um sprite para o score");
        }
    }
}

void ScoreManager::increaseScore() {
    currentScore++;
}

void ScoreManager::draw() const {
    // Centralizando o score
    float currentScoreX = BUFFER_W / 2.0f;
    float currentScoreY = 10.0f;

    drawNumberSprites(currentScore, currentScoreX, currentScoreY);
}


void ScoreManager::drawNumberSprites(int number, int drawX, int drawY) const {
    if (number < 0) number = 0;

    // 1. FATOR DE ESCALA
    // Defina o quão pequeno você quer o placar. 1.0f é o tamanho original,
    const float scale = 0.75f;

    std::string s = std::to_string(number);
    
    // 2. CALCULA A LARGURA TOTAL JÁ COM A ESCALA APLICADA
    // Isso é crucial para a centralização correta.
    float totalScaledWidth = 0;
    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            // Soma a largura de cada dígito, já multiplicada pela escala.
            totalScaledWidth += al_get_bitmap_width(digitSprites[digit]) * scale;
        }
    }

    // 3. CENTRALIZA COM BASE NA LARGURA TOTAL ESCALADA
    // Calcula a posição inicial X para que o número como um todo fique centralizado em drawX.
    float currentDigitX = drawX - (totalScaledWidth / 2.0f);

    // 4. DESENHA CADA DÍGITO DE FORMA ESCALADA
    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            ALLEGRO_BITMAP* digitSprite = digitSprites[digit];

            // Pega as dimensões originais do sprite do dígito
            float original_w = al_get_bitmap_width(digitSprite);
            float original_h = al_get_bitmap_height(digitSprite);

            // Calcula as novas dimensões escaladas
            float scaled_w = original_w * scale;
            float scaled_h = original_h * scale;
            
            // Usa a função de desenho escalado
            al_draw_scaled_bitmap(digitSprite,
                0, 0,           // Posição de origem no sprite (canto superior esquerdo)
                original_w,     // Largura original do sprite
                original_h,     // Altura original do sprite
                currentDigitX,  // Posição X de destino na tela
                drawY,          // Posição Y de destino na tela
                scaled_w,       // Largura final (escalada) na tela
                scaled_h,       // Altura final (escalada) na tela
                0               // Flags
            );
            
            // Avança a posição X para o próximo dígito usando a LARGURA ESCALADA.
            currentDigitX += scaled_w;
        }
    }
}

void ScoreManager::reset() {
    currentScore = 0;
}