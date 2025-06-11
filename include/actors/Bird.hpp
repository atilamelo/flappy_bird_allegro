#pragma once

#include "actors/GameObject.hpp"

class Bird : public GameObject
{
    float velocityY;

public:
    Bird(float x, float y, float w, float h, float vy);

    void update(float deltaTime) override;
    void draw(float deltaTime) override;
    void jump();
};