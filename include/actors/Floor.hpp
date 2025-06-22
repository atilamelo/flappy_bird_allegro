#pragma once

#include <allegro5/allegro.h>
#include "core/GameObject.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"


class Floor : public GameObject, public IDrawable, public IUpdatable
{
private:
    ALLEGRO_BITMAP* texture;
    
public:
    Floor(ALLEGRO_BITMAP* texture);

    void draw() const override;
    void update(float deltaTime) override;
};