#pragma once
#include "Pipe.hpp"

class PipePair
{
private:
    Pipe topPipe;    // The upper pipe in the pair
    Pipe bottomPipe; // The lower pipe in the pair
    float gap;       // The vertical gap between the top and bottom pipes
    float speed;     // Speed at which the pipes move
    float x;         // X position of the pipe pair
public:
    PipePair(float x, float startYGap, float gap, float speed);

    float getX() const;
    float getGap() const;
    float getSpeed() const;

    void draw(float deltaTime);
    void update(float deltaTime);
};