#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    ALLEGRO_DISPLAY	*display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_EVENT_QUEUE *eventQueue = nullptr;
    ALLEGRO_FONT *font = nullptr;
    

    bool done = false;
    bool redraw = true;
    long frameCount = 0;

    bool init();
    void loadAssets();

    // -- Loop de lógica do jogo --
    void processEvent(ALLEGRO_EVENT& event);
    void update();
    void render();
};