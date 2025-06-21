#pragma once

#include <actors/GameObject.hpp>
#include <allegro5/allegro.h>

class Floor : public GameObject {
private:
    ALLEGRO_BITMAP* texture;
    
public:
    Floor(ALLEGRO_BITMAP* texture);

    void draw() override;
    void update(float deltaTime) override;
};