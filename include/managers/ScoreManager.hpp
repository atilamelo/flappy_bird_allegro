#pragma once

#include "interfaces/IDrawable.hpp"
#include <allegro5/allegro.h>

class ScoreManager : public IDrawable
{
public:
    ScoreManager();

    void increaseScore();
    void draw() const override;
    void reset();

private:
    int currentScore = 0;
    ALLEGRO_BITMAP* digitSprites[10];

    void drawNumberSprites(int number, int drawX, int drawY) const;
};