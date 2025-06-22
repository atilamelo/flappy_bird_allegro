#include "actors/PipePair.hpp"
#include "Constants.hpp"
#include "util/ResourceManager.hpp"
#include "actors/Bird.hpp"

PipePair::PipePair() : topPipe(0, 0, 0, 0, 0, PipeType::TOP, nullptr), 
                             bottomPipe(0, 0, 0, 0, 0, PipeType::BOTTOM, nullptr) {}


void PipePair::init(float x, float startYGap, float gap, float speed) {
    this->x = x;
    this->gap = gap;
    this->speed = speed;


    ALLEGRO_BITMAP* pipeTexture = ResourceManager::getInstance().getBitmap("pipe-green");
    topPipe = Pipe(x, 0, PIPE_WIDTH, startYGap, speed, PipeType::TOP, pipeTexture);
    bottomPipe = Pipe(x, startYGap + gap, PIPE_WIDTH, PLAYABLE_AREA_HEIGHT - (startYGap + gap), speed, PipeType::BOTTOM, pipeTexture);
    
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

    topPipe.setX(this->x);
    bottomPipe.setX(this->x);
}

bool PipePair::isColliding(const Bird& bird) {
    if (!active) return false;

    // Bird's bounding box
    float birdLeft = bird.getX();
    float birdRight = bird.getX() + bird.getWidth();
    float birdTop = bird.getY();
    float birdBottom = bird.getY() + bird.getHeight();

    // Top pipe's bounding box
    float topPipeLeft = topPipe.getX();
    float topPipeRight = topPipe.getX() + topPipe.getWidth();
    float topPipeTop = topPipe.getY();
    float topPipeBottom = topPipe.getY() + topPipe.getHeight();

    // Bottom pipe's bounding box
    float bottomPipeLeft = bottomPipe.getX();
    float bottomPipeRight = bottomPipe.getX() + bottomPipe.getWidth();
    float bottomPipeTop = bottomPipe.getY();
    float bottomPipeBottom = bottomPipe.getY() + bottomPipe.getHeight();

    // Check collision with top pipe
    bool collidesTop = birdRight > topPipeLeft &&
                       birdLeft < topPipeRight &&
                       birdTop < topPipeBottom &&
                       birdBottom > topPipeTop;

    // Check collision with bottom pipe
    bool collidesBottom = birdRight > bottomPipeLeft &&
                          birdLeft < bottomPipeRight &&
                          birdTop < bottomPipeBottom &&
                          birdBottom > bottomPipeTop;

    return collidesTop || collidesBottom;
}

void PipePair::reset() {
    active = false;
}