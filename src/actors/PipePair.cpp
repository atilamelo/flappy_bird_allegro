#include <actors/PipePair.hpp>
#include "Constants.hpp"

PipePair::PipePair() : topPipe(0, 0, 0, 0, 0, PipeType::TOP), 
                             bottomPipe(0, 0, 0, 0, 0, PipeType::BOTTOM) {}


void PipePair::init(float x, float startYGap, float gap, float speed) {
    this->x = x;
    this->gap = gap;
    this->speed = speed;

    topPipe = Pipe(x, 0, PIPE_WIDTH, startYGap, speed, PipeType::TOP);
    bottomPipe = Pipe(x, startYGap + gap, PIPE_WIDTH, BUFFER_H - (startYGap + gap), speed, PipeType::BOTTOM);
    
    active = true;
    passed = false;
}

void PipePair::draw() {
    if (!active) return;

    topPipe.draw();
    bottomPipe.draw();
}

bool PipePair::hasPassed(const Bird& bird) {
    if (!active) return false;

    if (bird.getX() > x + PIPE_WIDTH && !passed) {
        passed = true;
        return true;
    }

    return false;
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