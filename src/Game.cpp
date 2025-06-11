#include "Game.hpp"
#include "Constants.hpp"
#include "actors/Bird.hpp"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>

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
    must_init(al_install_keyboard(), "keyboard");
}

Game::~Game() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_font(font);

    for(GameObject* gameObject : gameObjects) {
        delete gameObject;
    }

    gameObjects.clear();
}

bool Game::init() {
    timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");
    
    queue = al_create_event_queue();
    must_init(queue, "queue");
    
    display = al_create_display(BUFFER_W, BUFFER_H);
    must_init(display, "display");
    
    font = al_create_builtin_font();
    must_init(font, "font");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    

    gameObjects.push_back(new Bird(50, 50, 20, 20, 100));
    return 1;

}

void Game::loadAssets() {

}


void Game::run() {
    init();
    
    lastUpdate = std::chrono::steady_clock::now();
    
    al_start_timer(timer);
    while(!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        
        processEvent(event);
        
        if (redraw && al_is_event_queue_empty(queue)) {
            auto now = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
            
            update();
            draw();

            redraw = false;
            lastUpdate = now;
        }
    }
}

void Game::processEvent(ALLEGRO_EVENT& event) {
    switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        case ALLEGRO_EVENT_TIMER:            
            redraw = true;
            frameCount++;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                for(GameObject* gameObject : gameObjects) {
                    if(Bird* bird = dynamic_cast<Bird*>(gameObject)) {
                        bird->jump();
                    }
                }
            }
    }

    
}

void Game::update() {
    for(GameObject* gameObject : gameObjects) {
        gameObject->update(this->deltaTime);
    }
}

void Game::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    for(GameObject* gameObject : gameObjects) {
        gameObject->draw(this->deltaTime);
    }

    al_flip_display();

    redraw = true;
}


