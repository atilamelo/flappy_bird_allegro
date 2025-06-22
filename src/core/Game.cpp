/**
 * @file Game.cpp
 * @brief Implementação da classe principal Game.
 */
#include "core/Game.hpp"
#include "core/GameScene.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <chrono>
#include <algorithm>

namespace {
    void must_init(bool test, const char* description) {
        if (test) return;
        printf("Não pode inicializar %s\n", description);
        exit(1);
    }
}

Game::Game() : display(nullptr), timer(nullptr), queue(nullptr), isRunning(false) {
    initialize();
}

Game::~Game() {
    shutdown();
}

void Game::initialize() {
    must_init(al_init(), "Allegro");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font addon");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "image addon");

    timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "event queue");

    display = al_create_display(BUFFER_W, BUFFER_H);
    must_init(display, "display");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    // Carrega recursos globais
    ResourceManager::getInstance().loadAtlasJson("assets/sprites/sprite_sheet.json", "atlas", "assets/sprites/sprite_sheet.png");

    // Prepara a cena inicial
    sceneManager.set_current_scene(std::make_unique<GameScene>(&sceneManager));

    isRunning = true;
}

void Game::run() {
    al_start_timer(timer);
    bool redraw = true;
    auto lastUpdateTime = std::chrono::steady_clock::now();

    while (isRunning) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        processEvent(event);

        // A lógica de update com delta time variável foi movida para cá.
        if (event.type == ALLEGRO_EVENT_TIMER) {
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = currentTime - lastUpdateTime;
            float deltaTime = elapsed.count();
            lastUpdateTime = currentTime;
            
            const float max_deltaTime = 1.0f / 30.0f;
            if (deltaTime > max_deltaTime) {
                deltaTime = max_deltaTime;
            }
            
            update(deltaTime);
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            draw();
        }
    }
}

void Game::processEvent(const ALLEGRO_EVENT& event) {
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        isRunning = false;
    }
    sceneManager.processEvent(event);
}

void Game::update(float deltaTime) {
    sceneManager.update(deltaTime);
    if (!sceneManager.is_running()) {
        isRunning = false;
    }
}

void Game::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    sceneManager.draw();
    al_flip_display();
}

void Game::shutdown() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}