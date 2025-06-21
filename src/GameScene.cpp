#include "GameScene.hpp"
#include "Constants.hpp"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <vector>
#include "SceneManager.hpp"
#include "actors/PipePair.hpp"
#include "util/ResourceManager.hpp"

GameScene::GameScene(SceneManager *sceneManager)
    : Scene(sceneManager),
      pipePool(2),
      scoreManager()
{
    std::vector<ALLEGRO_BITMAP *> bird_frames = {
        ResourceManager::getInstance().getBitmap("yellowbird-downflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-midflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-upflap")};

    bird = std::make_unique<Bird>(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT, bird_frames);

    ALLEGRO_BITMAP *background_texture = ResourceManager::getInstance().getBitmap("background-day");
    background = std::make_unique<ParallaxBackground>(0, 0, BUFFER_W, BUFFER_H, background_texture, BACKGROUND_SCROLL_SPEED);

    ALLEGRO_BITMAP *floor_texture = ResourceManager::getInstance().getBitmap("base");
    floor = std::make_unique<Floor>(floor_texture);

    state = GameState::GAME_INIT;
}

void GameScene::loadAssets()
{
}

void GameScene::processEvent(const ALLEGRO_EVENT &event)
{
    switch (event.type)
    {
    case ALLEGRO_EVENT_KEY_DOWN:
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
        {
            if (state == GameState::GAME_INIT)
            {
                state = GameState::PLAYING;
                bird->setPhysicEnabled(true);
            }
            bird->jump();
        }
        break;
    }
}

void GameScene::update(float deltaTime)
{

    bird->update(deltaTime);
    background->update(deltaTime);
    floor->update(deltaTime);

    if (state == GameState::PLAYING)
    {
        if (timeSinceLastPipe >= PIPE_INTERVAL)
        {
            int maxGapStart = static_cast<int>(PLAYABLE_AREA_HEIGHT - PIPE_MIN_HEIGHT - PIPE_GAP);
            float startYGap = static_cast<float>(rand() % maxGapStart);

            PipePair *newPipePair = pipePool.getPipe();
            if (newPipePair)
                newPipePair->init(BUFFER_W, startYGap, PIPE_GAP, PIPE_SPEED);
            timeSinceLastPipe = 0.0f;
        }

        if (bird->getY() > PLAYABLE_AREA_HEIGHT - bird->getHeight())
        {
            gameOver();
            std::cout << "Colidiu com o chão" << std::endl;
        }

        pipePool.update(deltaTime);
        for (auto &pipePair : pipePool.getPipes())
        {
            if (pipePair->isColliding(*bird))
            {
                std::cout << "Colisão detectada!" << std::endl;
                gameOver();
                return;
            }

            if (pipePair->hasPassed(*bird))
            {
                std::cout << "Pássaro passou pelo tubo!" << std::endl;
                scoreManager.increaseScore();
            }
        }
        timeSinceLastPipe += deltaTime;
    }
}

void GameScene::gameOver()
{
    sceneManager->set_current_scene(std::make_unique<GameScene>(sceneManager));
}

void GameScene::draw()
{
    background->draw();
    pipePool.draw();
    bird->draw();
    scoreManager.draw();
    floor->draw();
}