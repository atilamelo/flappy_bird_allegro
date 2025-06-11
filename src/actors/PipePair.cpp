#include <actors/PipePair.hpp>
#include "Constants.hpp"

PipePair::PipePair(float x, float startYgap, float gap, float speed) : topPipe(x, 0, PIPE_WIDTH, startYgap, speed),
    bottomPipe(x, startYgap + gap, PIPE_WIDTH, BUFFER_H - (startYgap + gap), speed),
          gap(gap), speed(speed), x(x) {}


float PipePair::getX() const {
    return x;
}

float PipePair::getGap() const {
    return gap;
}

float PipePair::getSpeed() const {
    return speed;
}

void PipePair::draw(float deltaTime) {
    topPipe.draw(deltaTime);
    bottomPipe.draw(deltaTime);
}

void PipePair::update(float deltaTime) {
    topPipe.update(deltaTime);
    bottomPipe.update(deltaTime);
}