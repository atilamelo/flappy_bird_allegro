#pragma once

#include "actors/GameObject.hpp"

class Bird : public GameObject
{
    float velocityY;

public:
    Bird(float x, float y, float w, float h);

    void update(float deltaTime) override;
    void draw(float deltaTime) override;
    void jump();
};