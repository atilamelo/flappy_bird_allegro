#pragma once

#include "actors/GameObject.hpp"
#include <vector>
#include <allegro5/allegro.h>

class Bird : public GameObject
{
private:
    float velY;
    std::vector<ALLEGRO_BITMAP *> frames;
    int currentFrameIndex = 0;
    float frameTime = 0.1f; // Tempo para trocar de frame
    float timeSinceLastFrame = 0.0f;
    float angle = 0.0f;
    float deltaAngle = 0.0f;

    // -- FLAGS --
    bool physicsEnabled = false;
    bool hoverEnabled = false;

public:
    Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP *> frames);

    void update(float deltaTime) override;
    void draw() override;
    void jump();

    void setPhysicEnabled(bool enable) { this->physicsEnabled = enable; };
};