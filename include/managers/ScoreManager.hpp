/**
 * @file ScoreManager.hpp
 * @brief Define o gerenciador de pontuação e renderização de números.
 */
#pragma once

#include "interfaces/IDrawable.hpp"
#include <allegro5/allegro.h>

/**
 * @enum TextAlign
 * @brief Define os possíveis modos de alinhamento horizontal para o texto/números.
 */
enum class TextAlign {
    LEFT,   ///< O 'x' fornecido é o canto esquerdo do primeiro dígito.
    CENTER, ///< O 'x' fornecido é o ponto central do número inteiro.
    RIGHT   ///< O 'x' fornecido é o canto direito do último dígito.
};

/**
 * @class ScoreManager
 * @brief Gerencia a pontuação do jogo e fornece uma interface para desenhar números na tela.
 */
class ScoreManager : public IDrawable {
private:
    int currentScore;
    ALLEGRO_BITMAP* digitSprites[10];

public:
    ScoreManager();

    void increaseScore() { currentScore++; }
    void reset() { currentScore = 0; }
    int getScore() const { return currentScore; }

    void draw() const override;

    /**
     * @brief Utilitário público para desenhar qualquer número com alinhamento customizável.
     * @param number O número a ser desenhado.
     * @param x A coordenada X de referência para o alinhamento.
     * @param y A coordenada Y do topo dos dígitos.
     * @param scale O fator de escala para o tamanho dos dígitos.
     * @param align O modo de alinhamento (LEFT, CENTER, ou RIGHT). O padrão é LEFT.
     */
    void drawNumberSprites(int number, float x, float y, float scale, TextAlign align = TextAlign::LEFT) const;
};