#include "Game.hpp"
#include "Constants.hpp"
#include "Input.hpp"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

// Função auxiliar para verificar a inicialização de componentes
void must_init(bool test, const char* description) {
    if (test) return;
    printf("couldn't initialize %s\n", description);
    exit(1);
}

Game::Game() {
    must_init(al_init(), "Allegro");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font addon");
}

Game::~Game() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_font(font);
}

bool Game::init() {

    return 1;

}

void Game::loadAssets() {

}


void Game::run() {
    timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");

    eventQueue = al_create_event_queue();
    must_init(eventQueue, "event queue");

    display = al_create_display(BUFFER_W, BUFFER_H);
    must_init(display, "display");

    font = al_create_builtin_font();
    must_init(font, "font");

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    while(!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);
        
        processEvent(event);

        if (redraw && al_is_event_queue_empty(eventQueue)) {
            update();
            render();
            redraw = false;
        }
    }
}

void Game::processEvent(ALLEGRO_EVENT& event) {
    Input::update(&event);
    switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            frameCount++;
            if(Input::isKeyPressed(ALLEGRO_EVENT_DISPLAY_CLOSE)) {
                done = true;
            }
            break;
    }
    
}

void Game::update() {
    
}

void Game::render() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
    al_flip_display();

    redraw = true;
}


