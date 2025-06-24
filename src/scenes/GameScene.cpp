/**
 * @file GameScene.cpp
 * @brief Implementação da classe GameScene utilizando uma arquitetura baseada em interfaces.
 */
#include "scenes/GameScene.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "managers/ResourceManager.hpp"
#include "managers/SceneManager.hpp"
#include "core/GameSound.hpp"
#include "actors/SoundButton.hpp"
#include <algorithm>
#include "actors/ui/GameOverScreen.hpp"
#include "core/PlayerData.hpp"

GameScene::GameScene(SceneManager* sceneManager, Theme theme)
    : Scene(sceneManager),
      pipePool(PIPE_POOL_SIZE),
      rng(std::random_device{}()),
      dist(0.0f, 1.0f)
{
    // --- 1. Criação dos Objetos ---
    bird = std::make_unique<Bird>(BIRD_START_X, BIRD_START_Y, BIRD_WIDTH, BIRD_HEIGHT, theme.bird_frames);
    
    background = std::make_unique<ParallaxBackground>(theme.background, BACKGROUND_SCROLL_SPEED);
    floor = std::make_unique<Floor>(theme.floor);
    flashEffect = std::make_unique<SplashScreen>(0.5f, al_map_rgb(255, 255, 255)); // Flash branco de meio segundo
    gameOverScreen = std::make_unique<GameOverScreen>(100.0f, 0.7f, ResourceManager::getInstance().getBitmap("gameover"));
    getReadyUI = std::make_unique<GetReadyUI>();
    scoreBoard = std::make_unique<ScoreBoard>(180.0f, 0.6f, scoreManager, 2.0f);

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
  
    // Carregando as imagens do botão de som
    ALLEGRO_BITMAP *img_on = al_load_bitmap("assets/sprites/som_0.bmp");
    ALLEGRO_BITMAP *img_off = al_load_bitmap("assets/sprites/som_1.bmp");

    gSound = std::make_unique<GameSound>();
    gSound->init();
    soundButton = std::make_unique<SoundButton>(10, 10, 20, 20, img_on, img_off, gSound.get());

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
    updatables.push_back(gameOverScreen.get());
    updatables.push_back(getReadyUI.get());
    updatables.push_back(scoreBoard.get());

    // Ordem é importante aqui, define em que ordem vai ser desenhado
    drawables.push_back(background.get());
    drawables.push_back(&pipePool);
    drawables.push_back(floor.get());
    drawables.push_back(bird.get());
    drawables.push_back(&scoreManager);
    if(soundButton) 
    drawables.push_back(soundButton.get()); 
    drawables.push_back(flashEffect.get());
    drawables.push_back(gameOverScreen.get());
    drawables.push_back(getReadyUI.get());
    drawables.push_back(scoreBoard.get()); 
}

void GameScene::processEvent(const ALLEGRO_EVENT& event) {
     if (soundButton)
        soundButton->processEvent(event);

    if (event.type != ALLEGRO_EVENT_KEY_DOWN || event.keyboard.keycode != ALLEGRO_KEY_SPACE) return;

    switch (state) {
        case GameState::GAME_INIT:
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
            gameOverScreen->startAnimation();
            bird->setPhysicsEnabled(false);
        }
    }     else if (state == GameState::GAME_OVER) {
        if (gameOverScreen->isAnimationFinished() && !scoreBoard->isActive()) {
            // Aqui você precisaria ter uma forma de buscar a melhor pontuação
            // (ex: de um arquivo ou de uma variável estática).
            // Por enquanto, usaremos um valor fixo como exemplo.
            int best = 50; // TODO: Implementar lógica de Best Score
            scoreBoard->show(scoreManager.getScore(), best);
        }
    }

}

// O loop de draw agora é simples e genérico, e respeita a ordem das camadas.
void GameScene::draw() const {
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
            gSound->play_point();
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
        gSound->play_hit();
        state = GameState::DYING;
        bird->die();
        gSound->play_death();
        flashEffect->trigger();

        // Remove os objetos que não devem mais ser atualizados durante a sequência de morte.
        updatables.erase(std::remove(updatables.begin(), updatables.end(), &pipePool), updatables.end()); // Canos param de se mexer
        updatables.erase(std::remove(updatables.begin(), updatables.end(), background.get()), updatables.end()); // Fundo para de rolar
        updatables.erase(std::remove(updatables.begin(), updatables.end(), floor.get()), updatables.end());     // Chão para de rolar

        // Remove a pontuação
        drawables.erase(std::remove(drawables.begin(), drawables.end(), &scoreManager), drawables.end());

        // Registra pontuacao do jogador
        PlayerData::getInstance().setScore(scoreManager.getScore());
    }
}

void GameScene::restart() {
    bird->reset();
    pipePool.reset();
    scoreManager.reset();
    gameOverScreen->reset();
    flashEffect->reset();
    scoreBoard->reset(); 
    getReadyUI->show();

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