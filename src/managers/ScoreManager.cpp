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

void ScoreManager::draw() const {
    drawNumberSprites(currentScore, BUFFER_W / 2.0f, 10.0f, 0.75f, TextAlign::CENTER);
}


float ScoreManager::getNumberWidth(int number, float scale) const {
    if (number < 0) number = 0;

    std::string s = std::to_string(number);
    float totalScaledWidth = 0;
    
    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            totalScaledWidth += al_get_bitmap_width(digitSprites[digit]) * scale;
        }
    }
    return totalScaledWidth;
}

void ScoreManager::drawNumberSprites(int number, float x, float y, float scale, TextAlign align) const {
    if (number < 0) number = 0;

    std::string s = std::to_string(number);
    float totalScaledWidth = getNumberWidth(number, scale);

    // Etapa 2: Calcula a posição X inicial com base no modo de alinhamento.
    float startX = 0;
    switch (align) {
        case TextAlign::LEFT:
            // O desenho começa exatamente no 'x' fornecido.
            startX = x;
            break;
        case TextAlign::CENTER:
            // O desenho começa à esquerda do 'x' pela metade da largura total.
            startX = x - (totalScaledWidth / 2.0f);
            break;
        case TextAlign::RIGHT:
            // O desenho começa à esquerda do 'x' pela largura total.
            startX = x - totalScaledWidth;
            break;
    }

    // Etapa 3: O loop de desenho agora é único e simples, sempre da esquerda para a direita.
    float currentDigitX = startX;

    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            ALLEGRO_BITMAP* digitSprite = digitSprites[digit];
            float original_w = al_get_bitmap_width(digitSprite);
            float original_h = al_get_bitmap_height(digitSprite);
            float scaled_w = original_w * scale;
            float scaled_h = original_h * scale;
            
            al_draw_scaled_bitmap(digitSprite, 0, 0, original_w, original_h, currentDigitX, y, scaled_w, scaled_h, 0);
            currentDigitX += scaled_w;
        }
    }
}