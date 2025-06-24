/**
 * @file GameOverScreen.cpp
 * @brief Implementação da classe GameOverScreen, que gerencia a animação e exibição da tela de fim de jogo.
 * @details Esta classe controla uma máquina de estados de animação para apresentar o "Game Over",
 * o placar, as pontuações e a medalha conquistada de forma sequencial e suave.
 */
#include "actors/ui/GameOverScreen.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <cmath>


/**
 * @brief Construtor da tela de Game Over.
 * @details Carrega as texturas necessárias a partir do ResourceManager, calcula as dimensões
 * e as posições iniciais e finais dos elementos da UI para a animação.
 * @param scManager Uma referência constante para o ScoreManager, usada para desenhar os números.
 */
GameOverScreen::GameOverScreen(const ScoreManager& scManager)
    : scoreManagerRef(scManager),
      gameOverGO(0,0,0,0),
      scoreBoardGO(0,0,0,0)
{
    ResourceManager& rm = ResourceManager::getInstance();

    // Carrega as texturas que serão usadas na tela de Game Over.
    gameOverTexture = rm.getBitmap("gameover");
    boardTexture = rm.getBitmap("score_board");
    newTexture = rm.getBitmap("new_record");

    // Configura as dimensões e as posições finais dos elementos principais.
    if (gameOverTexture) {
        gameOverGO.setWidth(al_get_bitmap_width(gameOverTexture));
        gameOverGO.setHeight(al_get_bitmap_height(gameOverTexture));
        gameOverGO.setX((BUFFER_W - gameOverGO.getWidth()) / 2.0f); // Centraliza horizontalmente.
        this->gameOver_endY = 100.0f;
        this->gameOver_startY = -gameOverGO.getHeight(); // Inicia fora da tela, acima.
    }
    if (boardTexture) {
        // O placar é escalado de acordo com a constante global.
        scoreBoardGO.setWidth(al_get_bitmap_width(boardTexture) * SCOREBOARD_SCALE);
        scoreBoardGO.setHeight(al_get_bitmap_height(boardTexture) * SCOREBOARD_SCALE);
        scoreBoardGO.setX((BUFFER_W - scoreBoardGO.getWidth()) / 2.0f); // Centraliza horizontalmente.
        this->scoreBoard_endY = 180.0f;
        this->scoreBoard_startY = -scoreBoardGO.getHeight(); // Inicia fora da tela, acima.
    }

    this->animationDuration = 0.6f; // Duração para cada estágio da animação em segundos.
    reset(); // Inicializa a tela no estado padrão (inativo).
}

/**
 * @brief Inicia a sequência de animação da tela de Game Over.
 * @param score A pontuação final do jogador na partida.
 * @param best A melhor pontuação registrada até então.
 */
void GameOverScreen::startSequence(int score, int best) {
    this->finalScore = score;
    this->newBestScore = false; // Reseta a flag de novo recorde.
    if(score > best) {
        this->bestScore = score;
        this->newBestScore = true; // Marca que um novo recorde foi alcançado.
    } else {
        this->bestScore = best;
    }
    determineMedal(); // Seleciona a medalha com base na pontuação final.

    // Prepara a máquina de estados para iniciar a animação.
    this->elapsedTime = 0.0f;
    this->gameOverGO.setY(gameOver_startY);
    this->scoreBoardGO.setY(scoreBoard_startY);
    this->currentState = AnimationState::SLIDING_GAMEOVER_BITMAP;
}

/**
 * @brief Reseta a tela para o seu estado inicial e inativo.
 * @details Essencial para garantir que, ao iniciar um novo jogo, a tela esteja "limpa" e pronta.
 */
void GameOverScreen::reset() {
    this->currentState = AnimationState::INACTIVE;
    this->elapsedTime = 0.0f;
    this->gameOverGO.setY(gameOver_startY);
    this->scoreBoardGO.setY(scoreBoard_startY);
    this->currentMedal = nullptr;
    this->newBestScore = false;
}

/**
 * @brief Determina qual medalha o jogador ganhou com base na pontuação.
 * @details Carrega o bitmap da medalha apropriada ou define como nulo se nenhuma for ganha.
 */
void GameOverScreen::determineMedal() {
    ResourceManager& rm = ResourceManager::getInstance();
    if (finalScore >= 30) currentMedal = rm.getBitmap("gold_medal");
    else if (finalScore >= 20) currentMedal = rm.getBitmap("silver_medal");
    else if (finalScore >= 10) currentMedal = rm.getBitmap("bronze_medal");
    else currentMedal = nullptr;
}

/**
 * @brief Atualiza a lógica de animação da tela.
 * @details Controla a máquina de estados que anima a entrada do texto "Game Over" e do placar.
 * @param deltaTime O tempo decorrido desde o último quadro.
 */
void GameOverScreen::update(float deltaTime) {
    if (currentState == AnimationState::INACTIVE || currentState == AnimationState::FINISHED) return;

    elapsedTime += deltaTime;
    float progress = elapsedTime / animationDuration;
    if (progress > 1.0f) progress = 1.0f;

    // Utiliza uma função de easing (ease-out) para suavizar a chegada dos elementos.
    // sin(progress * PI/2) cria uma curva de animação que desacelera no final.
    float easedProgress = sin(progress * (ALLEGRO_PI / 2.0));

    // --- Máquina de Estados da Animação ---
    if (currentState == AnimationState::SLIDING_GAMEOVER_BITMAP) {
        // Interpola a posição Y do bitmap "Game Over".
        gameOverGO.setY(gameOver_startY + (gameOver_endY - gameOver_startY) * easedProgress);
        if (progress >= 1.0f) {
            // Animação concluída, transiciona para o próximo estado.
            currentState = AnimationState::SLIDING_SCOREBOARD;
            elapsedTime = 0.0f; // Reseta o tempo para a nova animação.
        }
    }
    else if (currentState == AnimationState::SLIDING_SCOREBOARD) {
        // Interpola a posição Y do placar.
        scoreBoardGO.setY(scoreBoard_startY + (scoreBoard_endY - scoreBoard_startY) * easedProgress);
        if (progress >= 1.0f) {
            // Animação final concluída.
            currentState = AnimationState::FINISHED;
        }
    }
}

/**
 * @brief Desenha os elementos da tela de Game Over.
 * @details A renderização é condicional ao estado atual da animação.
 */
void GameOverScreen::draw() const {
    if (currentState == AnimationState::INACTIVE) return;

    // 1. Desenha o bitmap "Game Over" durante sua animação e depois.
    if (gameOverTexture) {
        al_draw_bitmap(gameOverTexture, gameOverGO.getX(), gameOverGO.getY(), 0);
    }

    // 2. O placar só é desenhado após o "Game Over" começar a se mover para a posição.
    if (currentState == AnimationState::SLIDING_SCOREBOARD || currentState == AnimationState::FINISHED) {
        // 2a. Desenha o painel do placar, que também está sendo animado e é escalado.
        if (boardTexture) {
            al_draw_scaled_bitmap(boardTexture,
                                  0, 0, // Coordenadas de origem na textura
                                  al_get_bitmap_width(boardTexture), al_get_bitmap_height(boardTexture),
                                  scoreBoardGO.getX(), scoreBoardGO.getY(), // Posição de destino
                                  scoreBoardGO.getWidth(), scoreBoardGO.getHeight(), // Tamanho de destino (escalado)
                                  0);
        }

        // 2b. O conteúdo do placar (medalha, scores) só aparece quando a animação termina.
        if (currentState == AnimationState::FINISHED) {
            // Deslocamentos (offsets) relativos ao canto superior esquerdo do placar escalado.
            const float medalOffsetX = 13 * SCOREBOARD_SCALE, medalOffsetY = 21 * SCOREBOARD_SCALE;
            const float scoreOffsetX = 101 * SCOREBOARD_SCALE, scoreOffsetY = 17 * SCOREBOARD_SCALE;
            const float bestOffsetY = 38 * SCOREBOARD_SCALE;
            const float numberScale = 0.5f; // Escala para os números da pontuação.

            // Desenha a medalha, se houver.
            if (currentMedal) {
                al_draw_scaled_bitmap(currentMedal,
                                      0, 0,
                                      al_get_bitmap_width(currentMedal), al_get_bitmap_height(currentMedal),
                                      scoreBoardGO.getX() + medalOffsetX, scoreBoardGO.getY() + medalOffsetY,
                                      al_get_bitmap_width(currentMedal) * SCOREBOARD_SCALE, al_get_bitmap_height(currentMedal) * SCOREBOARD_SCALE,
                                      0);
            }

            // Se for um novo recorde, desenha o selo "NEW".
            if(newBestScore) {
                const float NEW_SCALE = 1.5f;
                const float scaledNewTextureWidth = al_get_bitmap_width(newTexture) * NEW_SCALE;
                const float scaledNewTextureHeight = al_get_bitmap_height(newTexture) * NEW_SCALE;

                // Lógica para posicionar o selo "NEW" à esquerda dos números do recorde.
                // 1. Encontra a borda direita da área dos números.
                const float bestScoreRightX = scoreBoardGO.getX() + scoreOffsetX;
                // 2. Calcula a largura total dos números do recorde para encontrar a borda esquerda.
                const float bestScoreLeftX = bestScoreRightX - scoreManagerRef.getNumberWidth(bestScore, numberScale);
                // 3. Posiciona o selo "NEW" à esquerda dos números, com um pequeno espaçamento.
                const float newBitmapX = bestScoreLeftX - scaledNewTextureWidth - 10.0f;

                // Lógica para centralizar verticalmente o selo "NEW" com os números.
                const float digitHeight = al_get_bitmap_height(ResourceManager::getInstance().getBitmap("0")) * numberScale;
                const float bestScoreTopY = scoreBoardGO.getY() + bestOffsetY;
                const float newBitmapY = bestScoreTopY + (digitHeight / 2.0f) - (scaledNewTextureHeight / 2.0f);

                al_draw_scaled_bitmap(newTexture,
                                      0, 0,
                                      al_get_bitmap_width(newTexture), al_get_bitmap_height(newTexture),
                                      newBitmapX, newBitmapY,
                                      scaledNewTextureWidth, scaledNewTextureHeight,
                                      0);
            }

            // Desenha a pontuação final e a melhor pontuação, alinhadas à direita.
            scoreManagerRef.drawNumberSprites(finalScore, scoreBoardGO.getX() + scoreOffsetX, scoreBoardGO.getY() + scoreOffsetY, numberScale, TextAlign::RIGHT);
            scoreManagerRef.drawNumberSprites(bestScore, scoreBoardGO.getX() + scoreOffsetX, scoreBoardGO.getY() + bestOffsetY, numberScale, TextAlign::RIGHT);
        }
    }
}