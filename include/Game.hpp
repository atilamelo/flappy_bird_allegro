#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "actors/GameObject.hpp"
#include "actors/PipePair.hpp"
#include "actors/Bird.hpp"
#include "util/PipePool.hpp"
#include <vector>
#include <chrono>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    ALLEGRO_DISPLAY	*display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_EVENT_QUEUE *queue = nullptr;
    ALLEGRO_FONT *font = nullptr;
    Bird *bird = nullptr;
    PipePool *pipePool = nullptr;
    float timeSinceLastPipe = 0.0f;
    
    std::chrono::steady_clock::time_point lastUpdate;
    float deltaTime;


    bool done = false;
    bool redraw = true;
    long frameCount = 0;

    bool init();
    void loadAssets();

    // -- Loop de lógica do jogo --
    void processEvent(ALLEGRO_EVENT& event);
    void update();
    void draw();
};