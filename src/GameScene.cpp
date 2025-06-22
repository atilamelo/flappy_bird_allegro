#include "GameScene.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "SceneManager.hpp"
#include "actors/PipePair.hpp"
#include "util/ResourceManager.hpp"
#include <vector>
#include <random>

GameScene::GameScene(SceneManager* sceneManager)
    : Scene(sceneManager),
      pipePool(PIPE_POOL_SIZE),
      scoreManager(),
      rng(std::random_device{}()),
      dist(0.0f, 1.0f)
{
    std::vector<ALLEGRO_BITMAP*> bird_frames = {
        ResourceManager::getInstance().getBitmap("yellowbird-downflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-midflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-upflap")
    };
    bird = std::make_unique<Bird>(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT, bird_frames);

    ALLEGRO_BITMAP* background_texture = ResourceManager::getInstance().getBitmap("background-day");
    background = std::make_unique<ParallaxBackground>(0, 0, BUFFER_W, BUFFER_H, background_texture, BACKGROUND_SCROLL_SPEED);

    ALLEGRO_BITMAP* floor_texture = ResourceManager::getInstance().getBitmap("base");
    floor = std::make_unique<Floor>(floor_texture);

    state = GameState::GAME_INIT;
}

void GameScene::processEvent(const ALLEGRO_EVENT& event) {
    if (event.type != ALLEGRO_EVENT_KEY_DOWN || event.keyboard.keycode != ALLEGRO_KEY_SPACE) {
        return;
    }

    switch (state) {
        case GameState::GAME_INIT:
            state = GameState::PLAYING;
            bird->setPhysicEnabled(true);
            bird->setHoverEnabled(false);
            bird->jump();
            break;
        case GameState::PLAYING:
            bird->jump();
            break;
        case GameState::GAME_OVER:
            break;
    }
}

// Main Update Loop (Dispatcher)
void GameScene::update(float deltaTime) {
    bird->update(deltaTime);

    if (state != GameState::GAME_OVER) {
        background->update(deltaTime);
        floor->update(deltaTime);
    }

    switch (state) {
        case GameState::PLAYING:
            updatePlaying(deltaTime);
            break;
        case GameState::GAME_OVER:
            updateGameOver(deltaTime);
            break;
        case GameState::GAME_INIT:
            // Bird hover is handled by its own update
            break;
    }
}

// Update logic for when the game is actively being played
void GameScene::updatePlaying(float deltaTime) {
    timeSinceLastPipe += deltaTime;
    if (timeSinceLastPipe >= PIPE_INTERVAL) {
        spawnPipe();
        timeSinceLastPipe = 0.0f;
    }

    pipePool.update(deltaTime);
    checkCollisions();
    
    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() && pipePair->hasPassed(*bird)) {
            scoreManager.increaseScore();
        }
    }
}

// Update logic for the game over screen
void GameScene::updateGameOver(float deltaTime) {
    timeSinceDying += deltaTime;
    if (timeSinceDying > GAME_OVER_DELAY) {
        gameOver();
    }

    if (bird->isPhysicEnabled() && bird->getY() >= (PLAYABLE_AREA_HEIGHT - bird->getHeight())) {
        bird->setY(PLAYABLE_AREA_HEIGHT - bird->getHeight());
        bird->setPhysicEnabled(false);
    }
}

void GameScene::checkCollisions() {
    // Colisão com o chão
    if (bird->getY() > PLAYABLE_AREA_HEIGHT - bird->getHeight()) {
        enterGameOverState();
        return; // Não precisa checar outras colisões
    }
    
    // Colisão com o cano
    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() && pipePair->isColliding(*bird)) {
            enterGameOverState();
            return;
        }
    }
}

void GameScene::spawnPipe() {
    PipePair* newPipePair = pipePool.getPipe();
    if (newPipePair) {
        int maxGapStart = static_cast<int>(PLAYABLE_AREA_HEIGHT - PIPE_MIN_HEIGHT - PIPE_GAP);
        
        float startYGap = dist(rng) * maxGapStart;

        newPipePair->init(BUFFER_W, startYGap, PIPE_GAP, PIPE_SPEED);
    }
}

void GameScene::enterGameOverState() {
    state = GameState::GAME_OVER;
    bird->die();
    bird->setVelY(0);
}

void GameScene::gameOver() {
    sceneManager->set_current_scene(std::make_unique<GameScene>(sceneManager));
}

void GameScene::draw() {
    background->draw();
    pipePool.draw();
    bird->draw();
    scoreManager.draw();
    floor->draw();
}