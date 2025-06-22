#pragma once

#include <memory>
#include "Scene.hpp"
#include "Constants.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "actors/Floor.hpp"
#include "util/PipePool.hpp" 
#include "ScoreManager.hpp"

enum GameState {
    GAME_INIT,
    PLAYING,
    GAME_OVER
};

class GameScene : public Scene {
private:
    PipePool pipePool;
    ScoreManager scoreManager;
    GameState state;
    
    std::unique_ptr<Bird> bird;
    std::unique_ptr<ParallaxBackground> background;
    std::unique_ptr<Floor> floor;

    float timeSinceLastPipe = 0.0f;
    float timeSinceDying = 0.0f;

public:
    GameScene(SceneManager *sceneManager);

    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void gameOver();
    void draw() override;
};