/**
 * @file GameOverScreen.cpp
 * @brief Implementação da classe unificada GameOverScreen com escala no placar.
 */
#include "actors/ui/GameOverScreen.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <cmath>


GameOverScreen::GameOverScreen(const ScoreManager& scManager)
    : scoreManagerRef(scManager),
      gameOverGO(0,0,0,0),
      scoreBoardGO(0,0,0,0)
{
    ResourceManager& rm = ResourceManager::getInstance();

    // Carrega as texturas necessárias
    gameOverTexture = rm.getBitmap("gameover");
    boardTexture = rm.getBitmap("score_board");
    newTexture = rm.getBitmap("new_button");

    // Configura as dimensões e posições finais dos elementos
    if (gameOverTexture) {
        gameOverGO.setWidth(al_get_bitmap_width(gameOverTexture));
        gameOverGO.setHeight(al_get_bitmap_height(gameOverTexture));
        gameOverGO.setX((BUFFER_W - gameOverGO.getWidth()) / 2.0f);
        this->gameOver_endY = 100.0f;
        this->gameOver_startY = -gameOverGO.getHeight();
    }
    if (boardTexture) {
        scoreBoardGO.setWidth(al_get_bitmap_width(boardTexture) * SCOREBOARD_SCALE);
        scoreBoardGO.setHeight(al_get_bitmap_height(boardTexture) * SCOREBOARD_SCALE);
        scoreBoardGO.setX((BUFFER_W - scoreBoardGO.getWidth()) / 2.0f);
        this->scoreBoard_endY = 180.0f;
        this->scoreBoard_startY = -scoreBoardGO.getHeight();
    }

    this->animationDuration = 0.6f; // Duração para cada estágio da animação
    reset();
}

void GameOverScreen::startSequence(int score, int best) {
    this->finalScore = score;
    if(score > best) {
        this->bestScore = score;
        this->newBestScore = true;
    } else {
        this->bestScore = best;
    }
    determineMedal();

    this->elapsedTime = 0.0f;
    this->gameOverGO.setY(gameOver_startY);
    this->scoreBoardGO.setY(scoreBoard_startY);
    this->currentState = AnimationState::SLIDING_GAMEOVER_BITMAP;
}

void GameOverScreen::reset() {
    this->currentState = AnimationState::INACTIVE;
    this->elapsedTime = 0.0f;
    this->gameOverGO.setY(gameOver_startY);
    this->scoreBoardGO.setY(scoreBoard_startY);
    this->currentMedal = nullptr;
}

void GameOverScreen::determineMedal() {
    ResourceManager& rm = ResourceManager::getInstance();
    if (finalScore >= 30) currentMedal = rm.getBitmap("gold_medal");
    else if (finalScore >= 20) currentMedal = rm.getBitmap("silver_medal");
    else if (finalScore >= 10) currentMedal = rm.getBitmap("bronze_medal");
    else currentMedal = nullptr;
}

void GameOverScreen::update(float deltaTime) {
    if (currentState == AnimationState::INACTIVE || currentState == AnimationState::FINISHED) return;

    elapsedTime += deltaTime;
    float progress = elapsedTime / animationDuration;
    if (progress > 1.0f) progress = 1.0f;

    // Animação com desaceleração suave na chegada (ease-out)
    float easedProgress = sin(progress * (ALLEGRO_PI / 2.0));

    if (currentState == AnimationState::SLIDING_GAMEOVER_BITMAP) {
        // Anima o bitmap "Game Over"
        gameOverGO.setY(gameOver_startY + (gameOver_endY - gameOver_startY) * easedProgress);
        if (progress >= 1.0f) {
            // Quando terminar, inicia a próxima animação
            currentState = AnimationState::SLIDING_SCOREBOARD;
            elapsedTime = 0.0f; // Reseta o tempo para a animação do placar
        }
    }
    else if (currentState == AnimationState::SLIDING_SCOREBOARD) {
        // Anima o painel de score
        scoreBoardGO.setY(scoreBoard_startY + (scoreBoard_endY - scoreBoard_startY) * easedProgress);
        if (progress >= 1.0f) {
            // Quando terminar, a sequência está completa
            currentState = AnimationState::FINISHED;
        }
    }
}

void GameOverScreen::draw() const {
    if (currentState == AnimationState::INACTIVE) return;

    // 1. Desenha o bitmap "Game Over" em sua posição animada.
    if (gameOverTexture) {
        al_draw_bitmap(gameOverTexture, gameOverGO.getX(), gameOverGO.getY(), 0);
    }

    // 2. Só começa a desenhar o placar DEPOIS que a primeira animação começou.
    if (currentState == AnimationState::SLIDING_SCOREBOARD || currentState == AnimationState::FINISHED) {
        // 2a. Desenha o painel do placar em sua posição animada e escalado
        if (boardTexture) {
            al_draw_scaled_bitmap(boardTexture,
                                    0, 0,
                                    al_get_bitmap_width(boardTexture), al_get_bitmap_height(boardTexture),
                                    scoreBoardGO.getX(), scoreBoardGO.getY(),
                                    scoreBoardGO.getWidth(), scoreBoardGO.getHeight(),
                                    0);
        }

        // 2b. Desenha o conteúdo interno (medalha, scores) apenas quando o painel parar.
        if (currentState == AnimationState::FINISHED) {
            const float medalOffsetX = 13 * SCOREBOARD_SCALE, medalOffsetY = 21 * SCOREBOARD_SCALE;
            const float scoreOffsetX = 101 * SCOREBOARD_SCALE, scoreOffsetY = 17 * SCOREBOARD_SCALE;
            const float bestOffsetY = 38 * SCOREBOARD_SCALE;
            const float numberScale = 0.5f;

            if (currentMedal) {
                al_draw_scaled_bitmap(currentMedal,
                                        0, 0,
                                        al_get_bitmap_width(currentMedal), al_get_bitmap_height(currentMedal),
                                        scoreBoardGO.getX() + medalOffsetX, scoreBoardGO.getY() + medalOffsetY,
                                        al_get_bitmap_width(currentMedal) * SCOREBOARD_SCALE, al_get_bitmap_height(currentMedal) * SCOREBOARD_SCALE,
                                        0);
            }

            if(newBestScore) {
                // Largura escalada do bitmap 'newTexture'
                const float NEW_SCALE = 1.5f;
                const float scaledNewTextureWidth = al_get_bitmap_width(newTexture) * NEW_SCALE;

                // Calcula a coordenada X da direita onde os números do best score serão desenhados.
                const float bestScoreRightX = scoreBoardGO.getX() + scoreOffsetX;

                // Calcula a coordenada X da esquerda dos números do best score, considerando alinhamento à direita.
                const float bestScoreLeftX = bestScoreRightX - scoreManagerRef.getNumberWidth(bestScore, numberScale);

                const float newBitmapX = bestScoreLeftX - scaledNewTextureWidth - 10.0f;

                const float digitHeight = al_get_bitmap_height(ResourceManager::getInstance().getBitmap("0")) * numberScale;
                const float scaledNewTextureHeight = al_get_bitmap_height(newTexture) * NEW_SCALE;
                const float bestScoreTopY = scoreBoardGO.getY() + bestOffsetY;

                const float newBitmapY = bestScoreTopY + (digitHeight / 2.0f) - (scaledNewTextureHeight / 2.0f);

                al_draw_scaled_bitmap(newTexture,
                                      0, 0,
                                      al_get_bitmap_width(newTexture), al_get_bitmap_height(newTexture),
                                      newBitmapX, newBitmapY,
                                      scaledNewTextureWidth, al_get_bitmap_height(newTexture) * NEW_SCALE,
                                      0);

            }

            scoreManagerRef.drawNumberSprites(finalScore, scoreBoardGO.getX() + scoreOffsetX, scoreBoardGO.getY() + scoreOffsetY, numberScale, TextAlign::RIGHT);
            scoreManagerRef.drawNumberSprites(bestScore, scoreBoardGO.getX() + scoreOffsetX, scoreBoardGO.getY() + bestOffsetY, numberScale, TextAlign::RIGHT);
        }
    }
}