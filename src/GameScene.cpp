#include "GameScene.hpp"
#include "Constants.hpp"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include "SceneManager.hpp"

GameScene::GameScene(SceneManager *sceneManager) 
    : Scene(sceneManager),
      bird(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT),
      pipePool(2),
      timeSinceLastPipe(0.0f)
{
    loadAssets();
}
void GameScene::loadAssets() {
}

void GameScene::processEvent(const ALLEGRO_EVENT& event) {
    switch (event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                bird.jump();
            }
            break;
    }
}

void GameScene::update(float deltaTime) {
    if(timeSinceLastPipe >= PIPE_INTERVAL) {
        int maxGapStart = static_cast<int>(BUFFER_H - PIPE_MIN_HEIGHT - PIPE_GAP);
        float startYGap = static_cast<float>(rand() % maxGapStart);
        
        PipePair* newPipePair = pipePool.getPipe(); 
        if (newPipePair) 
             newPipePair->init(BUFFER_W, startYGap, PIPE_GAP, PIPE_SPEED);
        timeSinceLastPipe = 0.0f;
    }


    bird.update(deltaTime);
    pipePool.update(deltaTime);

    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() &&
            bird.getX() + bird.getWidth() > pipePair->getX() &&
            bird.getX() < pipePair->getX() + PIPE_WIDTH &&
            (bird.getY() < pipePair->getTopPipe().getY() + pipePair->getTopPipe().getHeight() ||
             bird.getY() + bird.getHeight() > pipePair->getBottomPipe().getY())) {
            std::cout << "Colisão detectada!" << std::endl;
            gameOver();
            return;
        }
    }
    timeSinceLastPipe += deltaTime;
}

void GameScene::gameOver() {
    sceneManager->set_current_scene(std::make_unique<GameScene>(sceneManager));
}

void GameScene::draw() {    
    pipePool.draw();
    bird.draw();
}