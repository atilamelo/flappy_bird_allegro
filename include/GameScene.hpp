#pragma once

#include "Scene.hpp"
#include "Constants.hpp"
#include "actors/Bird.hpp"
#include "util/PipePool.hpp" 

class GameScene : public Scene {
private:
    Bird bird;
    PipePool pipePool;
    
    float timeSinceLastPipe;

public:
    GameScene(SceneManager *sceneManager);

    void loadAssets() override;
    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void gameOver();
    void draw() override;
};