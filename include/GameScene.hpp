#pragma once

#include <memory>
#include "Scene.hpp"
#include "Constants.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "actors/Floor.hpp"
#include "util/PipePool.hpp" 
#include "ScoreManager.hpp"

class GameScene : public Scene {
private:
    PipePool pipePool;
    ScoreManager scoreManager;
    
    std::unique_ptr<Bird> bird;
    std::unique_ptr<ParallaxBackground> background;
    std::unique_ptr<Floor> floor;

    float timeSinceLastPipe = 0.0f;

public:
    GameScene(SceneManager *sceneManager);

    void loadAssets() override;
    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void gameOver();
    void draw() override;
};