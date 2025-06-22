#pragma once

#include "Scene.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "actors/Floor.hpp"
#include "util/PipePool.hpp"
#include "ScoreManager.hpp"
#include <memory>
#include <random>

class GameScene : public Scene {
public:
    explicit GameScene(SceneManager* sceneManager);

    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void draw() override;

private:
    // Game State
    enum class GameState {
        GAME_INIT,
        PLAYING,
        GAME_OVER
    };
    GameState state = GameState::GAME_INIT;

    // Game Objects
    std::unique_ptr<Bird> bird;
    std::unique_ptr<ParallaxBackground> background;
    std::unique_ptr<Floor> floor;
    PipePool pipePool;
    ScoreManager scoreManager;
    
    // Modificações de estados
    void updatePlaying(float deltaTime);
    void updateGameOver(float deltaTime);

    // Helper methods
    void spawnPipe();
    void checkCollisions();
    void enterGameOverState();
    void gameOver();

    // Timers
    float timeSinceLastPipe = 0.0f;
    float timeSinceDying = 0.0f;

    // RNG
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;
};