/**
 * @file GameScene.cpp
 * @brief Implementação da classe GameScene utilizando uma arquitetura baseada em interfaces.
 */
#include "core/GameScene.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "managers/ResourceManager.hpp"
#include "managers/SceneManager.hpp"
#include <algorithm> // Para std::find_if

GameScene::GameScene(SceneManager* sceneManager)
    : Scene(sceneManager),
      pipePool(PIPE_POOL_SIZE),
      rng(std::random_device{}()),
      dist(0.0f, 1.0f)
{
    // --- 1. Criação dos Objetos ---
    // A propriedade dos objetos é mantida em unique_ptr ou como membros diretos.

    std::vector<ALLEGRO_BITMAP*> bird_frames = {
        ResourceManager::getInstance().getBitmap("yellowbird-downflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-midflap"),
        ResourceManager::getInstance().getBitmap("yellowbird-upflap")
    };
    bird = std::make_unique<Bird>(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT, bird_frames);
    
    background = std::make_unique<ParallaxBackground>(ResourceManager::getInstance().getBitmap("background-day"), BACKGROUND_SCROLL_SPEED);
    floor = std::make_unique<Floor>(ResourceManager::getInstance().getBitmap("base"));
    flashEffect = std::make_unique<SplashScreen>(0.5f, al_map_rgb(255, 255, 255)); // Flash branco de meio segundo

    // --- 2. Construção das Listas de Interfaces ---
    // Populamos as listas de observadores para os loops polimórficos.
    buildEntityLists();
    
    // --- 3. Início do Jogo ---
    restart();
}

void GameScene::buildEntityLists() {
    // Adiciona objetos que são IUpdatable à lista de atualizáveis.
    // A ordem aqui não importa para a lógica de atualização.
    updatables.push_back(bird.get());
    updatables.push_back(background.get());
    updatables.push_back(floor.get());
    updatables.push_back(&pipePool);
    updatables.push_back(flashEffect.get()); 

    // Ordem é importante aqui, define em que ordem vai ser desenhado
    drawables.push_back(background.get());
    drawables.push_back(&pipePool);
    drawables.push_back(floor.get());
    drawables.push_back(bird.get());
    drawables.push_back(&scoreManager);
    drawables.push_back(flashEffect.get()); 
}

void GameScene::processEvent(const ALLEGRO_EVENT& event) {
    if (event.type != ALLEGRO_EVENT_KEY_DOWN || event.keyboard.keycode != ALLEGRO_KEY_SPACE) return;

    switch (state) {
        case GameState::GAME_INIT:
            state = GameState::PLAYING;
            bird->setPhysicsEnabled(true);
            bird->setHoverEnabled(false);
            bird->jump();
            break;
        case GameState::PLAYING:
            bird->jump();
            break;
        case GameState::GAME_OVER:
            restart();
            break;
        case GameState::DYING:
            break;
    }
}

// O loop de update agora é simples e genérico.
void GameScene::update(float deltaTime) {
    // 1. Atualiza todos os objetos que implementam IUpdatable.
    for (auto* updatable : updatables) {
        updatable->update(deltaTime);
    }
    
    // 2. Executa a lógica de jogo específica do estado atual.
    if (state == GameState::PLAYING) {
        updatePlaying(deltaTime);
    } else if (state == GameState::DYING) {
        // A lógica de morte agora pode ser integrada aqui ou em um método helper.
        if (bird->getY() >= (floor->getY() - bird->getHeight())) {
            state = GameState::GAME_OVER;
            bird->setPhysicsEnabled(false);
        }
    }
}

// O loop de draw agora é simples e genérico, e respeita a ordem das camadas.
void GameScene::draw() {
    // 1. Desenha todos os objetos que implementam IDrawable.
    for (const auto* drawable : drawables) {
        drawable->draw();
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
            scoreManager.increaseScore();
        }
    }
}

void GameScene::checkCollisions() {
    if (bird->getY() >= (floor->getY() - bird->getHeight()) || bird->getY() <= 0) {
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
        state = GameState::DYING;
        bird->die();
        flashEffect->trigger();

        // Remove os objetos que não devem mais ser atualizados durante a sequência de morte.
        updatables.erase(std::remove(updatables.begin(), updatables.end(), &pipePool), updatables.end()); // Canos param de se mexer
        updatables.erase(std::remove(updatables.begin(), updatables.end(), background.get()), updatables.end()); // Fundo para de rolar
        updatables.erase(std::remove(updatables.begin(), updatables.end(), floor.get()), updatables.end());     // Chão para de rolar
        
    }
}

void GameScene::restart() {
    bird->reset();
    pipePool.reset();
    scoreManager.reset();
    timeSinceLastPipe = 0.0f;
    
    // Reconstrói as listas para garantir que tudo está no estado correto (ex: pipePool volta a ser atualizável)
    updatables.clear();
    drawables.clear();
    buildEntityLists();

    state = GameState::GAME_INIT;
}

void GameScene::spawnPipe() {
    PipePair* newPipePair = pipePool.getPipe();
    if (newPipePair) {
        float playableHeight = floor->getY();
        int maxGapStart = static_cast<int>(playableHeight - PIPE_MIN_HEIGHT - PIPE_GAP);
        float startYGap = dist(rng) * maxGapStart;
        newPipePair->init(BUFFER_W, startYGap, PIPE_GAP, PIPE_SPEED);
    }
}