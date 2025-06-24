/**
 * @file GameScene.cpp
 * @brief Implementação da classe GameScene com lógica de estado segura e explícita.
 */
#include "scenes/GameScene.hpp"
#include "managers/ResourceManager.hpp"
#include "managers/SceneManager.hpp"
#include "scenes/StartMenu.hpp"
#include <iostream>
#include <string>
#include "util/ScoreSystem.hpp"
#include "core/PlayerData.hpp"
#include "widgetz/widgetz.h"

// O construtor permanece o mesmo, mas vamos usar o ResourceManager para os botões de som.
GameScene::GameScene(SceneManager* sceneManager, const Theme& selectedTheme)
    : Scene(sceneManager),
      pipePool(PIPE_POOL_SIZE),
      rng(std::random_device{}()),
      dist(0.0f, 1.0f),
      selectedTheme(selectedTheme)
{
    ResourceManager& rm = ResourceManager::getInstance();

    currentPipeTexture = selectedTheme.pipe;
    bird = std::make_unique<Bird>(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT, selectedTheme.bird_frames);
    background = std::make_unique<ParallaxBackground>(selectedTheme.background, BACKGROUND_SCROLL_SPEED);
    floor = std::make_unique<Floor>(selectedTheme.floor);
    
    scoreManager = std::make_unique<ScoreManager>();
    flashEffect = std::make_unique<SplashScreen>(0.4f, al_map_rgb(255, 255, 255));
    gameOverScreen = std::make_unique<GameOverScreen>(*scoreManager);
    getReadyUI = std::make_unique<GetReadyUI>();
    
    gSound = std::make_unique<GameSound>();
    gSound->init(selectedTheme.music_path);
    
    ALLEGRO_BITMAP *img_on = al_load_bitmap("assets/sprites/som_0.png");

    ALLEGRO_BITMAP *img_off = al_load_bitmap("assets/sprites/som_1.png");
    soundButton = std::make_unique<SoundButton>(10, 10, 20, 20, img_on, img_off, gSound.get());


    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    
    restart();
}


void GameScene::processEvent(const ALLEGRO_EVENT& event) {
    ALLEGRO_EVENT e = event;
    wz_send_event(gui, &e);
    // O botão de som sempre pode ser clicado.
    if (soundButton)
        soundButton->processEvent(event);

    if ((event.type != ALLEGRO_EVENT_KEY_DOWN || event.keyboard.keycode != ALLEGRO_KEY_SPACE) && state!=GameState::GAME_OVER) return;

    switch (state) {
        case GameState::GAME_INIT:
            initGUI();
            state = GameState::PLAYING;
            bird->setPhysicsEnabled(true);
            bird->setHoverEnabled(false);
            getReadyUI->hide(); 
            bird->jump();
            gSound->play_fly();
            break;
        case GameState::PLAYING:
            bird->jump();
            gSound->play_fly();
            break;
        case GameState::GAME_OVER:
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) restart();
            break;
        case GameState::DYING:
            break;
    }

        if (e.type == WZ_BUTTON_PRESSED) {
            sceneManager->setCurrentScene(std::make_unique<StartMenu>(sceneManager));
            if (gui) {
                std::cout << "Destruindo a GUI do Ranking." << std::endl;
                wz_destroy(gui);
            }
        }
}

void GameScene::update(float deltaTime) {
    switch (state) {
        case GameState::GAME_INIT:
            background->update(deltaTime);
            bird->update(deltaTime);
            floor->update(deltaTime);
            getReadyUI->update(deltaTime);
            break;

        case GameState::PLAYING:
            background->update(deltaTime);
            floor->update(deltaTime);
            pipePool.update(deltaTime);
            bird->update(deltaTime); // Pássaro executa a física normal
            updatePlaying(deltaTime); // Lógica de jogo (canos, colisões, score)
            break;

        case GameState::DYING:
            bird->update(deltaTime); // O pássaro continua caindo
            flashEffect->update(deltaTime); // O flash desaparece

            // A transição para GAME_OVER acontece quando o pássaro vai para fora da tela
            if (bird->getY() >= BUFFER_H  && flashEffect->isFinished()) {
                state = GameState::GAME_OVER;
                ScoreSystem& scoreSystem = ScoreSystem::getInstance();
                std::string name = PlayerData::getName();
                int actualScore = PlayerData::getScore();
                int bestScore = scoreSystem.getPlayerScore(name);

                gameOverScreen->startSequence(actualScore, bestScore);
                scoreSystem.registerOrUpdateScore(name, actualScore);
            }
            break;
        case GameState::GAME_OVER:
            gameOverScreen->update(deltaTime);
            wz_update(gui,deltaTime);
            break;
    }
}

void GameScene::draw() const {
    // Camada 1: Fundo
    background->draw();
    pipePool.draw();
    floor->draw();

    // Camada 2: Personagem
    bird->draw();
    
    // Camada 3: UI do Jogo
    if (state == GameState::PLAYING) {
        scoreManager->draw();
    }
    soundButton->draw();

    // Camada 4: UI de Estado
    getReadyUI->draw();
    gameOverScreen->draw();
    
    // Camada 5: Efeitos por cima de tudo
    flashEffect->draw();

    if (state == GameState::GAME_OVER) {
        wz_draw(gui);
    }

    
}

// --- MÉTODOS DE LÓGICA INTERNA ---

void GameScene::updatePlaying(float deltaTime) {
    timeSinceLastPipe += deltaTime;
    if (timeSinceLastPipe >= PIPE_INTERVAL) {
        spawnPipe();
        timeSinceLastPipe = 0.0f;
    }
    checkCollisions();
    
    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() && pipePair->hasPassed(*bird)) {
            gSound->play_point();
            scoreManager->increaseScore();
        }
    }
}

void GameScene::checkCollisions() {
    if (bird->getY() + bird->getHeight() >= floor->getY() || bird->getY() <= 0) {
        initiateDeathSequence();
        return;
    }
    
    for (auto& pipePair : pipePool.getPipes()) {
        if (pipePair->isActive() && pipePair->isColliding(*bird)) {
            initiateDeathSequence();
            return;
        }
    }
}

void GameScene::initiateDeathSequence() {
    if (state == GameState::PLAYING) {
        gSound->play_hit();
        state = GameState::DYING;
        bird->die();
        gSound->play_death();
        flashEffect->trigger();

        PlayerData::getInstance().setScore(scoreManager->getScore());
    }
}

void GameScene::restart() {
    if (gui) {
        std::cout << "Destruindo a GUI do Ranking." << std::endl;
        wz_destroy(gui);
    }
    bird->reset();
    pipePool.reset();
    scoreManager->reset();
    gameOverScreen->reset();
    flashEffect->reset();
    getReadyUI->show();
    timeSinceLastPipe = 0.0f;
    state = GameState::GAME_INIT;
    initGUI();
}

void GameScene::spawnPipe() {
    PipePair* newPipePair = pipePool.getPipe();
    if (newPipePair) {
        float playableHeight = floor->getY();
        int maxGapStart = static_cast<int>(playableHeight - PIPE_MIN_HEIGHT - PIPE_GAP);
        float startYGap = dist(rng) * maxGapStart;
        newPipePair->init(BUFFER_W, startYGap, PIPE_GAP, PIPE_SPEED, selectedTheme.pipe);
    }
}

void GameScene::initGUI(){
    ResourceManager& rm = ResourceManager::getInstance();
    // Configuração da GUI (WidgetZ)
    memset(&skin_theme, 0, sizeof(skin_theme));
    memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME*)&skin_theme);
    float button_w = 80.0f;
    float button_h = 28.0f;
    float button_x = (BUFFER_W - button_w) / 2.0f;
    scoreboard_h;
    buttons_y = 350;
    wz_create_image_button(gui, button_x, buttons_y, button_w, button_h, rm.getBitmap("btn_menu_normal"), rm.getBitmap("btn_menu_pressed"), rm.getBitmap("btn_menu_focused"), nullptr, 1); // Voltar Página

    ALLEGRO_EVENT_QUEUE* queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);

}