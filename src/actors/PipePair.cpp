#include <actors/PipePair.hpp>
#include "Constants.hpp"
#include "util/ResourceManager.hpp"

PipePair::PipePair() : topPipe(0, 0, 0, 0, 0, PipeType::TOP, nullptr), 
                             bottomPipe(0, 0, 0, 0, 0, PipeType::BOTTOM, nullptr) {}


void PipePair::init(float x, float startYGap, float gap, float speed) {
    this->x = x;
    this->gap = gap;
    this->speed = speed;


    ALLEGRO_BITMAP* pipeTexture = ResourceManager::getInstance().getBitmap("pipe-green");
    topPipe = Pipe(x, 0, PIPE_WIDTH, startYGap, speed, PipeType::TOP, pipeTexture);
    bottomPipe = Pipe(x, startYGap + gap, PIPE_WIDTH, BUFFER_H - (startYGap + gap), speed, PipeType::BOTTOM, pipeTexture);
    
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