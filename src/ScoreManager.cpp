#include <ScoreManager.hpp>
#include <iostream>
#include "util/ResourceManager.hpp"
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

void ScoreManager::draw() {
    // Centralizando o score
    float currentScoreX = BUFFER_W / 2.0f;
    float currentScoreY = 50.0f;

    drawNumberSprites(currentScore, currentScoreX, currentScoreY);
}


void ScoreManager::drawNumberSprites(int number, int drawX, int drawY) {
    if (number < 0) number = 0;

    std::string s = std::to_string(number);
    
    int currentDigitX = drawX;

    int totalWidth = 0;
    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            totalWidth += al_get_bitmap_width(digitSprites[digit]);
        }
    }
    currentDigitX = drawX - (totalWidth / 2.0f);

    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            al_draw_bitmap(digitSprites[digit], currentDigitX, drawY, 0);
            currentDigitX += al_get_bitmap_width(digitSprites[digit]);
        }
    }
}
