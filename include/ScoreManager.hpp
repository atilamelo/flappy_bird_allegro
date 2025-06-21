#pragma once

#include <allegro5/allegro.h>

class ScoreManager
{
public:
    ScoreManager();

    void increaseScore();
    void draw();

private:
    int currentScore = 0;
    ALLEGRO_BITMAP* digitSprites[10];

    void drawNumberSprites(int number, int drawX, int drawY);
};