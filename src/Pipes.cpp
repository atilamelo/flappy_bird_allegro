#include "actors/GameObject.hpp"

class Pipes : public GameObject {
float velocityX;

public:
    Pipes(float x, float y, float w, float h, float velocityX)
        : GameObject(x, y, w, h), velocityX(velocityX) {}

    void draw(float deltaTime) override {
        // Implement drawing logic for pipes
    }

    void update(float deltaTime) override {
        // Update the position of the pipes based on velocity
        x += velocityX * deltaTime;
    }
};