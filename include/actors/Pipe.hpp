#pragma once
#include <actors/GameObject.hpp>
#include <allegro5/allegro_primitives.h>

enum class PipeType {
    TOP,
    BOTTOM
};

class Pipe : public GameObject {
private:
    float speed;
    ALLEGRO_BITMAP* texture;
    PipeType pipeType;
    
public:
    Pipe(float x, float y, float width, float height, float speed, PipeType pipeType, ALLEGRO_BITMAP* texture);

    void draw() override;
    void update(float deltaTime) override;
    
    float getSpeed() const;
    void setSpeed(float speed);
};