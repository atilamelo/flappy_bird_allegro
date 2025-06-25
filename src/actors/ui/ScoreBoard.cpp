/**
 * @file ScoreBoard.cpp
 * @brief Implementação da classe ScoreBoard.
 */
#include "actors/ui/ScoreBoard.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <cmath>

ScoreBoard::ScoreBoard(float finalY, float duration, const ScoreManager& scManager, float initialScale)
    : GameObject((BUFFER_W - al_get_bitmap_width(ResourceManager::getInstance().getBitmap("score_board")) * initialScale) / 2.0f, 0, 0, 0),
      scoreManager(scManager),
      currentScale(initialScale)
{
    // Carrega a textura principal do painel
    boardTexture = ResourceManager::getInstance().getBitmap("score_board");
    if(boardTexture) {
        this->width = al_get_bitmap_width(boardTexture);
        this->height = al_get_bitmap_height(boardTexture);
    }
    
    // Configurações da animação
    this->animationDuration = duration;
    this->endY = finalY;
    this->startY = -this->height * this->currentScale; // Começa totalmente acima da tela, considerando a escala

    reset();
}

void ScoreBoard::show(int score, int best) {
    this->finalScore = score;
    this->bestScore = best;
    determineMedal();
    
    // Inicia a animação
    this->elapsedTime = 0.0f;
    this->y = this->startY;
    this->active = true;
}

void ScoreBoard::reset() {
    this->active = false;
    this->elapsedTime = 0.0f;
    this->y = startY;
    this->currentMedal = nullptr;
}

void ScoreBoard::determineMedal() {
    ResourceManager& rm = ResourceManager::getInstance();
    
    // Lógica para definir a medalha. Ajuste os valores como desejar.
    if (finalScore >= 30) {
        currentMedal = rm.getBitmap("gold_medal");
    } else if (finalScore >= 20) {
        currentMedal = rm.getBitmap("silver_medal");
    } else if (finalScore >= 10) {
        currentMedal = rm.getBitmap("bronze_medal");
    }
}

void ScoreBoard::update(float deltaTime) {
    if (!active || elapsedTime >= animationDuration) return;

    elapsedTime += deltaTime;
    float progress = elapsedTime / animationDuration;
    if (progress > 1.0f) progress = 1.0f;

    // Animação com desaceleração suave (Ease-Out)
    float easedProgress = 1.0f - pow(1.0f - progress, 3.0f); // Cubic ease-out

    // Interpola a posição Y
    // A interpolação é feita entre startY e endY, que já foram definidos considerando a escala se necessário.
    this->y = startY + (endY - startY) * easedProgress;
}

void ScoreBoard::draw() const {
    if (!active) return;

    // 1. Desenha o painel de fundo
    if (boardTexture) {
        al_draw_scaled_bitmap(boardTexture, 
                              0, 0,
                              al_get_bitmap_width(boardTexture), al_get_bitmap_height(boardTexture),
                              x, y, // Destination X, Y
                              width * currentScale, height * currentScale, // Destination W, H (aplicando escala)
                              0); // Flags
    }

    // 2. Desenha a medalha (se houver) dentro do painel
    if (currentMedal) {
        const float medalOffsetX = 13;
        const float medalOffsetY = 21;
        al_draw_scaled_bitmap(currentMedal,
                              0, 0, // Source X, Y
                              al_get_bitmap_width(currentMedal), al_get_bitmap_height(currentMedal), // Source W, H
                              x + (medalOffsetX * currentScale), y + (medalOffsetY * currentScale), // Destination X, Y (offsets escalados)
                              al_get_bitmap_width(currentMedal) * currentScale, al_get_bitmap_height(currentMedal) * currentScale, // Destination W, H (aplicando escala)
                              0); // Flags
    }
 
}

void ScoreBoard::setScale(float newScale) {
    this->currentScale = newScale;
    this->startY = -this->height * this->currentScale;
    this->x = (BUFFER_W - (al_get_bitmap_width(boardTexture) * currentScale)) / 2.0f;
}