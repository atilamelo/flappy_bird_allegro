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
    /// @brief Função utilitária para verificar se uma inicialização do Allegro foi bem-sucedida.
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
    // --- Inicialização de todos os módulos do Allegro ---
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

    // --- Registro das fontes de eventos ---
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
        
    // --- Carregamento de Recursos Globais ---
    // Todos os assets usados em múltiplas cenas são pré-carregados aqui para evitar
    // acessos a disco durante a transição de cenas.
    try {
        ResourceManager::getInstance().loadAtlasJson("assets/sprites/sprite_sheet.json", "atlas", "assets/sprites/sprite_sheet.png");
        ResourceManager& rm = ResourceManager::getInstance();

        rm.loadAtlasJson("assets/sprites/sprite_sheet.json", "atlas", "assets/sprites/sprite_sheet.png");
        rm.loadAtlasJson("assets/sprites/sprite_sheet_ui.json", "atlasUI", "assets/sprites/sprite_sheet_ui.png");
        rm.loadBitmap("icon", "assets/sprites/icon.png");

        // -- Som --
        rm.loadSample("point", "assets/audio/point.wav");
        rm.loadSample("die", "assets/audio/die.wav");
        rm.loadSample("hit", "assets/audio/hit.wav");
        rm.loadSample("fly", "assets/audio/wing.wav");
        rm.loadAudioStream("8bitMusicTheme", "assets/audio/8bit.ogg", 4, 4096);
        rm.loadAudioStream("starMusicTheme", "assets/audio/star.ogg", 4, 4096);
        rm.loadAudioStream("barbie", "assets/audio/barbie.ogg", 4, 4096);
        rm.loadAudioStream("yoshi", "assets/audio/yoshi.ogg", 4, 4096);
        
        std::cout << "Recursos carregados com sucesso." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro fatal ao carregar recursos: " << e.what() << std::endl;
        exit(-1);
    }

    
    // --- Configurações de janela ---

    // 1. Obtém as informações do monitor principal (monitor 0).
    ALLEGRO_MONITOR_INFO monitor_info;
    al_get_monitor_info(0, &monitor_info);

    // 2. Calcula a largura e altura do monitor a partir das informações obtidas.
    int monitor_w = monitor_info.x2 - monitor_info.x1;
    int monitor_h = monitor_info.y2 - monitor_info.y1;

    // 3. Calcula a posição X e Y para a janela ficar centralizada.
    int window_x = (monitor_w - BUFFER_W) / 2;
    int window_y = (monitor_h - BUFFER_H) / 2;

    // 4. Define a posição da janela com as coordenadas calculadas.
    al_set_window_position(display, window_x, window_y);
    
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_window_title(display, "Flappy Bird | Clone - PDSII");
    al_set_display_icon(display, ResourceManager::getInstance().getBitmap("icon"));

    // --- Setup da Cena Inicial ---
    // O jogo começa no menu principal.
    sceneManager.setEventQueue(queue);
    sceneManager.setCurrentScene(std::make_unique<StartMenu>(&sceneManager));

    isRunning = true;
}

void Game::run() {
    al_start_timer(timer);
    bool redraw = true;
    auto lastUpdateTime = std::chrono::steady_clock::now();

    // --- Loop Principal do Jogo (Game Loop) ---
    while (isRunning) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        processEvent(event);

        // A atualização da lógica (update) é atrelada ao timer do Allegro,
        // mas usa um deltaTime variável para movimentos mais suaves e consistentes.
        if (event.type == ALLEGRO_EVENT_TIMER) {
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = currentTime - lastUpdateTime;
            float deltaTime = elapsed.count();
            lastUpdateTime = currentTime;
            
            // Medida de segurança para evitar o "espiral da morte": se o deltaTime for
            // muito grande (ex: lag), ele é limitado para não quebrar a física do jogo.
            const float max_deltaTime = 1.0f / 30.0f;
            if (deltaTime > max_deltaTime) {
                deltaTime = max_deltaTime;
            }
            
            update(deltaTime);
            redraw = true; // Marca que uma atualização de lógica ocorreu, então precisamos redesenhar.
        }

        // A renderização (draw) só ocorre se a fila de eventos estiver vazia e
        // a flag 'redraw' estiver ativa. Isso evita renderizações desnecessárias.
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

    if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
        al_acknowledge_resize(display);
    }


    // Delega o processamento de eventos para a cena ativa.
    sceneManager.processEvent(event);
}

void Game::update(float deltaTime) {
    // Delega a atualização da lógica para a cena ativa.
    sceneManager.update(deltaTime);
    if (!sceneManager.isRunning()) {
        isRunning = false;
    }
}

void Game::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    // Delega a renderização para a cena ativa.
    sceneManager.draw();
    al_flip_display();
}

void Game::shutdown() {
    // Destrói os recursos do Allegro na ordem inversa da criação.
    if (display) {
        al_destroy_display(display);
    }
    if (timer) {
        al_destroy_timer(timer);
    }
    if (queue) {
        al_destroy_event_queue(queue);
    }

    // Desinstala os addons do Allegro.
    al_uninstall_mouse();
    al_uninstall_audio();
    al_uninstall_keyboard();
    al_shutdown_primitives_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
}