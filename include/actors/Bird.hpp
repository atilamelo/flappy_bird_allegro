#pragma once

#include "actors/GameObject.hpp"
#include <vector>
#include <allegro5/allegro.h>

class Bird : public GameObject
{
    float velocityY;
    std::vector<ALLEGRO_BITMAP*> frames;
    int currentFrameIndex = 0;
    float frameTime = 0.1f; // Tempo para trocar de frame
    float timeSinceLastFrame = 0.0f;

public:
    Bird(float x, float y, float w, float h, std::vector<ALLEGRO_BITMAP*> frames);

    void update(float deltaTime) override;
    void draw() override;
    void jump();
};