/**
 * @file GameScene.cpp
 * @brief Implementação dos métodos da cena principal do jogo.
 */
#include "GameScene.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "util/ResourceManager.hpp"
#include "SceneManager.hpp"

GameScene::GameScene(SceneManager* sceneManager)
    : Scene(sceneManager),
      pipePool(PIPE_POOL_SIZE),
      rng(std::random_device{}()), // Semeia o gerador aleatório
      dist(0.0f, 1.0f)            // Distribuição para gerar valores entre 0.0 e 1.0
{
    std::vector<ALLEGRO_BITMAP*> bird_frames = {
        ResourceManager::getInstance().getBitmap("yellowbird-downflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-midflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-upflap")
    };
    bird = std::make_unique<Bird>(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT, bird_frames);

    ALLEGRO_BITMAP* background_texture = ResourceManager::getInstance().getBitmap("background-day");
    background = std::make_unique<ParallaxBackground>(background_texture, BACKGROUND_SCROLL_SPEED);

    ALLEGRO_BITMAP* floor_texture = ResourceManager::getInstance().getBitmap("base");
    floor = std::make_unique<Floor>(floor_texture);
    
    restart();
}

void GameScene::processEvent(const ALLEGRO_EVENT& event) {
    if (event.type != ALLEGRO_EVENT_KEY_DOWN || event.keyboard.keycode != ALLEGRO_KEY_SPACE) {
        return;
    }

    switch (state) {
        case GameState::GAME_INIT:
            // Primeiro pulo inicia o jogo
            state = GameState::PLAYING;
            bird->setPhysicsEnabled(true);
            bird->setHoverEnabled(false);
            bird->jump();
            break;
        case GameState::PLAYING:
            bird->jump();
            break;
        case GameState::GAME_OVER:
            // No fim de jogo, espaço reinicia
            restart();
            break;
        case GameState::DYING:
            // Nenhuma ação durante a queda
            break;
    }
}

// A função update principal agora é um "despachante" limpo.
void GameScene::update(float deltaTime) {
    // Bird é atualizado em quase todos os estados, então sua chamada pode ficar aqui.
    // A lógica interna do pássaro decidirá o que fazer com base em suas flags.
    bird->update(deltaTime);

    // Cenário só para de se mover no fim de jogo definitivo.
    if (state != GameState::GAME_OVER) {
        background->update(deltaTime);
        floor->update(deltaTime);
    }
    
    // Despacha para a lógica de update específica do estado.
    switch (state) {
        case GameState::PLAYING:
            updatePlaying(deltaTime);
            break;
        case GameState::DYING:
            updateDying(deltaTime);
            break;
        case GameState::GAME_INIT:
        case GameState::GAME_OVER:
            // Nenhuma lógica de jogo adicional nesses estados.
            break;
    }
}

void GameScene::draw() {
    background->draw();
    pipePool.draw();
    floor->draw();
    bird->draw();
    scoreManager.draw();
}

// --- MÉTODOS DE LÓGICA INTERNA ---

void GameScene::updatePlaying(float deltaTime) {
    pipePool.update(deltaTime);
    
    timeSinceLastPipe += deltaTime;
    if (timeSinceLastPipe >= PIPE_INTERVAL) {
        spawnPipe();
        timeSinceLastPipe = 0.0f;
    }

    checkCollisions();
    
    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() && pipePair->hasPassed(*bird)) {
            scoreManager.increaseScore();
        }
    }
}

void GameScene::updateDying(float deltaTime) {
    // Durante a queda, apenas verificamos se o pássaro tocou o chão para
    // finalmente terminar o jogo.
    if (bird->getY() >= (floor->getY() - bird->getHeight())) {
        state = GameState::GAME_OVER;
        // Trava o pássaro no chão
        bird->setY(floor->getY() - bird->getHeight());
        bird->setPhysicsEnabled(false);
    }
}

void GameScene::checkCollisions() {
    // Colisão com o chão ou teto
    if (bird->getY() >= (floor->getY() - bird->getHeight()) || bird->getY() <= 0) {
        initiateDeathSequence();
        return;
    }
    
    // Colisão com os canos
    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() && pipePair->isColliding(*bird)) {
            initiateDeathSequence();
            return;
        }
    }
}

void GameScene::spawnPipe() {
    PipePair* newPipePair = pipePool.getPipe();
    if (newPipePair) {
        float playableHeight = floor->getY();
        int maxGapStart = static_cast<int>(playableHeight - PIPE_MIN_HEIGHT - PIPE_GAP);
        float startYGap = dist(rng) * maxGapStart;
        newPipePair->init(BUFFER_W, startYGap, PIPE_GAP, PIPE_SPEED);
    }
}

void GameScene::initiateDeathSequence() {
    // Garante que a sequência só aconteça uma vez
    if (state == GameState::PLAYING) {
        state = GameState::DYING;
        bird->die();
    }
}

void GameScene::restart() {
    // Reseta todos os objetos para seus estados iniciais
    bird->reset();
    pipePool.reset();
    scoreManager.reset();
    timeSinceLastPipe = 0.0f;

    // Coloca o jogo de volta no estado inicial
    state = GameState::GAME_INIT;
}