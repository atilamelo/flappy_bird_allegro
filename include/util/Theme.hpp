#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <vector>

struct Theme {
    std::vector<ALLEGRO_BITMAP*> bird_frames;
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* floor;
    ALLEGRO_BITMAP* pipe;
    ALLEGRO_SAMPLE* music;
};