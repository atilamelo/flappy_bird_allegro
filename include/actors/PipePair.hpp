#pragma once
#include "Pipe.hpp"

class PipePair
{
private:
    bool active = false; // Indicates if the pipe pair is active
    Pipe topPipe;    // The upper pipe in the pair
    Pipe bottomPipe; // The lower pipe in the pair
    float gap;       // The vertical gap between the top and bottom pool
    float speed;     // Speed at which the pool move
    float x;         // X position of the pipe pair
public:
    PipePair();

    float getX() const { return x; }
    float getGap() const { return gap; }
    float getSpeed() const { return speed; }
    bool isActive() const { return active; }

    Pipe getTopPipe() const { return topPipe; }
    Pipe getBottomPipe() const { return bottomPipe; }

    void init(float x, float startYGap, float gap, float speed);


    void draw();
    void update(float deltaTime);
};