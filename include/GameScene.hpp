#pragma once

#include <memory>
#include "Scene.hpp"
#include "Constants.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "util/PipePool.hpp" 

class GameScene : public Scene {
private:
    std::unique_ptr<Bird> bird;
    PipePool pipePool;
    std::unique_ptr<ParallaxBackground> background;
    
    float timeSinceLastPipe;

public:
    GameScene(SceneManager *sceneManager);

    void loadAssets() override;
    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void gameOver();
    void draw() override;
};