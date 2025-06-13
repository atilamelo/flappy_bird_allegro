#pragma once
#include <actors/GameObject.hpp>

class Pipe : public GameObject {
private:
    float speed;
public:
    Pipe(float x, float y, float width, float height, float speed);

    void draw() override;
    void update(float deltaTime) override;
    
    float getSpeed() const;
    void setSpeed(float speed);
};