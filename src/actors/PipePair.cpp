#include <actors/PipePair.hpp>
#include "Constants.hpp"

PipePair::PipePair() : topPipe(0, 0, 0, 0, 0), 
                             bottomPipe(0, 0, 0, 0, 0) {}


void PipePair::init(float x, float startYGap, float gap, float speed) {
    this->x = x;
    this->gap = gap;
    this->speed = speed;

    topPipe = Pipe(x, 0, PIPE_WIDTH, startYGap, speed);
    bottomPipe = Pipe(x, startYGap + gap, PIPE_WIDTH, BUFFER_H - (startYGap + gap), speed);
    
    active = true;
}

void PipePair::draw(float deltaTime) {
    if (!active) return;

    topPipe.draw(deltaTime);
    bottomPipe.draw(deltaTime);
}

void PipePair::update(float deltaTime) {
    if (!active) return;
    
    x -= speed * deltaTime;

    if(x + PIPE_WIDTH < 0) {
        active = false;
        return;
    }

    topPipe.update(deltaTime);
    bottomPipe.update(deltaTime);
}