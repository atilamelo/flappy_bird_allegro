#pragma once
#include "actors/Pipe.hpp"
#include "actors/Bird.hpp"

class PipePair
{
private:
    bool active = false; // Indicates if the pipe pair is active
    Pipe topPipe;    // The upper pipe in the pair
    Pipe bottomPipe; // The lower pipe in the pair
    float gap;       // The vertical gap between the top and bottom pool
    float speed;     // Speed at which the pool move
    float x;         // X position of the pipe pair
    bool passed = false; // Indicates if the bird has passed the pipe pair
public:
    PipePair();

    float getX() const { return x; }
    float getGap() const { return gap; }
    float getSpeed() const { return speed; }
    bool isActive() const { return active; }
    bool hasPassed(const Bird& bird);
    bool isColliding(const Bird& bird);

    Pipe getTopPipe() const { return topPipe; }
    Pipe getBottomPipe() const { return bottomPipe; }

    void init(float x, float startYGap, float gap, float speed);

    void draw();
    void update(float deltaTime);
};