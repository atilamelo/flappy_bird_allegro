/**
 * @file Game.cpp
 * @brief Implementação da classe principal Game.
 */
#include "core/Game.hpp"
#include "scenes/GameScene.hpp"
#include "actors/SoundButton.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include "scenes/StartMenu.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
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
    must_init(al_install_audio(), "al_install_audio()");
    must_init(al_init_acodec_addon(), "al_init_acodec_addon()");
    must_init(al_reserve_samples(16), "al_reserve_samples()");
    must_init(al_install_mouse(), "al_install_mouse()");
    
    timer = al_create_timer(1.0 / FPS);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "event queue");

    display = al_create_display(BUFFER_W, BUFFER_H);
    must_init(display, "display");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    // Carrega recursos globais
    ResourceManager::getInstance().loadAtlasJson("assets/sprites/sprite_sheet.json", "atlas", "assets/sprites/sprite_sheet.png");
    ResourceManager& rm = ResourceManager::getInstance();
    try {
        // Imagens do Menu
        rm.loadBitmap("logo_text", "data/flappy_bird.png");
        
        // Botões do Menu
        rm.loadBitmap("btn_start_normal", "data/home_start_button.png");
        rm.loadBitmap("btn_start_pressed", "data/home_start_button_pressed.png");
        rm.loadBitmap("btn_start_focused", "data/home_start_button_focused.png");
        
        rm.loadBitmap("btn_rank_normal", "data/rank_button.png");
        rm.loadBitmap("btn_rank_pressed", "data/rank_button_pressed.png");
        rm.loadBitmap("btn_rank_focused", "data/rank_button_focused.png");

        rm.loadBitmap("btn_quit_normal", "data/quit_button.png");
        rm.loadBitmap("btn_quit_pressed", "data/quit_button_pressed.png");
        rm.loadBitmap("btn_quit_focused", "data/quit_button_focused.png");

        rm.loadBitmap("edit_box", "data/editbox.png");
        rm.loadBitmap("get_ready", "assets/sprites/getready.png");
        
        std::cout << "Recursos carregados com sucesso." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro fatal ao carregar recursos: " << e.what() << std::endl;
        exit(-1);
    }

    // Prepara a cena inicial
    sceneManager.setEventQueue(queue);
    sceneManager.setCurrentScene(std::make_unique<StartMenu>(&sceneManager));

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
    if (!sceneManager.isRunning()) {
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