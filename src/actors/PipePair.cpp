/**
 * @file PipePair.cpp
 * @brief Implementação da classe PipePair.
 */
#include "actors/PipePair.hpp"
#include "actors/Bird.hpp"
#include "Constants.hpp"
#include "managers/ResourceManager.hpp"

PipePair::PipePair() 
    : GameObject(0, 0, PIPE_WIDTH, 0), // A altura do par não é relevante
      topPipe(0, 0, 0, PipeType::TOP, nullptr),
      bottomPipe(0, 0, 0, PipeType::BOTTOM, nullptr)
{
    reset();
}

void PipePair::init(float startX, float startYGap, float gapSize, float scrollSpeed)
{
    this->x = startX;
    this->speed = scrollSpeed;
    this->gap = gapSize;

    ALLEGRO_BITMAP* pipeTexture = ResourceManager::getInstance().getBitmap("pipe-green");

    topPipe = Pipe(0, PIPE_WIDTH, startYGap, PipeType::TOP, pipeTexture);
    bottomPipe = Pipe(startYGap + gap, PIPE_WIDTH, PLAYABLE_AREA_HEIGHT - (startYGap + gap), PipeType::BOTTOM, pipeTexture);

    this->active = true;
    this->passed = false;
}

void PipePair::reset()
{
    active = false;
    passed = false;
    x = -PIPE_WIDTH; // Move para fora da tela para evitar colisões acidentais
}

void PipePair::update(float deltaTime)
{
    if (!active) return;
    x -= speed * deltaTime;
    if (x + width < 0) {
        active = false;
    }
}

void PipePair::draw()
{
    if (!active) return;
    topPipe.draw(this->x);
    bottomPipe.draw(this->x);
}

bool PipePair::isColliding(const Bird& bird) const
{
    if (!active) return false;

    const float birdLeft = bird.getX();
    const float birdRight = bird.getX() + bird.getWidth();
    const float birdTop = bird.getY();
    const float birdBottom = bird.getY() + bird.getHeight();

    const float pipeLeft = this->x;
    const float pipeRight = this->x + this->width;

    // Colisão com cano superior
    if (birdRight > pipeLeft && birdLeft < pipeRight && birdTop < topPipe.getY() + topPipe.getHeight()) {
        return true;
    }
    // Colisão com cano inferior
    if (birdRight > pipeLeft && birdLeft < pipeRight && birdBottom > bottomPipe.getY()) {
        return true;
    }

    return false;
}

bool PipePair::hasPassed(const Bird& bird)
{
    if (!active || passed) return false;

    if (bird.getX() > this->x + this->width) {
        passed = true;
        return true;
    }
    return false;
}