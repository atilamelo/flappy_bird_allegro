#include "managers/ScoreManager.hpp"
#include <iostream>
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"

/**
 * @brief Construtor do ScoreManager.
 * @details Pega as imagens dos dígitos (0 a 9) que já foram carregadas no ResourceManager.
 * Assim, ele fica pronto para desenhar os números na tela quando for preciso.
 */
ScoreManager::ScoreManager() {
    for (int i = 0; i < 10; ++i) {
        std::string digitId = std::to_string(i);
        // Pega o bitmap correspondente ao dígito e guarda no nosso array.
        digitSprites[i] = ResourceManager::getInstance().getBitmap(digitId);
        if (!digitSprites[i]) {
            // Se por algum motivo a imagem do número não foi carregada, avisa e para o jogo.
            std::cerr << "AVISO: Falha ao carregar sprite para o dígito " << digitId << std::endl;
            throw std::runtime_error("Houve um erro ao carregar um sprite para o score");
        }
    }
}

/**
 * @brief Desenha a pontuação atual no local padrão do jogo (topo da tela).
 */
void ScoreManager::draw() const {
    // É um atalho para a função mais completa, usando valores padrão.
    drawNumberSprites(currentScore, BUFFER_W / 2.0f, 10.0f, 0.75f, TextAlign::CENTER);
}

/**
 * @brief Calcula a largura total (em pixels) que um número ocuparia se fosse desenhado.
 * @details Isso é super útil para centralizar ou alinhar números à direita.
 * @param number O número a ser medido.
 * @param scale A escala em que o número seria desenhado.
 * @return A largura total em pixels.
 */
float ScoreManager::getNumberWidth(int number, float scale) const {
    if (number < 0) number = 0;

    std::string s = std::to_string(number);
    float totalScaledWidth = 0;
    
    // Passa por cada dígito do número, somando a largura de cada imagem.
    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            totalScaledWidth += al_get_bitmap_width(digitSprites[digit]) * scale;
        }
    }
    return totalScaledWidth;
}

/**
 * @brief A função principal que desenha um número na tela usando sprites.
 * @param number O número a ser desenhado.
 * @param x A coordenada X de referência para o alinhamento.
 * @param y A coordenada Y onde o número será desenhado.
 * @param scale A escala do desenho (1.0 = tamanho original).
 * @param align O tipo de alinhamento (Esquerda, Centro, Direita).
 */
void ScoreManager::drawNumberSprites(int number, float x, float y, float scale, TextAlign align) const {
    if (number < 0) number = 0;

    std::string s = std::to_string(number);
    // Primeiro, calcula a largura total que o número vai ocupar.
    float totalScaledWidth = getNumberWidth(number, scale);

    // Agora, calcula a posição X onde o desenho do primeiro dígito deve começar,
    // com base no alinhamento que a gente quer.
    float startX = 0;
    switch (align) {
        case TextAlign::LEFT:
            // Alinhado à esquerda: começa exatamente no X que foi passado.
            startX = x;
            break;
        case TextAlign::CENTER:
            // Centralizado: começa um pouco antes, recuando metade da largura total.
            startX = x - (totalScaledWidth / 2.0f);
            break;
        case TextAlign::RIGHT:
            // Alinhado à direita: começa bem antes, recuando a largura inteira.
            startX = x - totalScaledWidth;
            break;
    }

    // Com o startX calculado, agora é só desenhar cada dígito, um depois do outro.
    float currentDigitX = startX;

    for (char c : s) {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            ALLEGRO_BITMAP* digitSprite = digitSprites[digit];
            float original_w = al_get_bitmap_width(digitSprite);
            float original_h = al_get_bitmap_height(digitSprite);
            float scaled_w = original_w * scale;
            float scaled_h = original_h * scale;
            
            // Desenha o sprite do dígito atual.
            al_draw_scaled_bitmap(digitSprite, 0, 0, original_w, original_h, currentDigitX, y, scaled_w, scaled_h, 0);
            
            // E avança a posição X para o próximo dígito.
            currentDigitX += scaled_w;
        }
    }
}