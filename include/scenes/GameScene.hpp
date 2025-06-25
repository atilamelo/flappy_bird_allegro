/**
 * @file GameScene.hpp
 * @brief Definição da cena principal do jogo, que gerencia todo o gameplay.
 * @version 4.0
 * @date 2025-06-22
 */
#pragma once

#include "core/Scene.hpp"
#include "util/Theme.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "actors/Floor.hpp"
#include "actors/PipePool.hpp"
#include "managers/ScoreManager.hpp"
#include "actors/effects/SplashScreen.hpp"
#include "actors/ui/GameOverScreen.hpp"
#include "actors/ui/GetReadyUI.hpp"
#include "actors/SoundButton.hpp"
#include "core/GameSound.hpp"
#include "widgetz/widgetz.h"
#include <memory>
#include <vector>
#include <random>

enum class GameState {
    GAME_INIT,      // Jogo na tela inicial, esperando o jogador.
    PLAYING,        // Jogo em andamento.
    DYING,          // Animação de morte do pássaro em andamento.
    GAME_OVER       // Tela de "Fim de Jogo", aguardando reinício.
};

/**
 * @class GameScene
 * @brief Orquestra todo o gameplay, gerenciando os objetos de jogo e a máquina de estados.
 */
class GameScene : public Scene
{
private:
    float scoreboard_h;
    float buttons_y;
    float spacing;

    // --- Entidades e Gerenciadores ---
    std::unique_ptr<Bird> bird;
    std::unique_ptr<ParallaxBackground> background;
    std::unique_ptr<Floor> floor;
    PipePool pipePool;
    std::unique_ptr<ScoreManager> scoreManager;
    std::unique_ptr<GameSound> gSound;

    // --- Elementos de UI e Efeitos ---
    std::unique_ptr<SoundButton> soundButton;
    std::unique_ptr<SplashScreen> flashEffect;
    std::unique_ptr<GameOverScreen> gameOverScreen;
    std::unique_ptr<GetReadyUI> getReadyUI;
    WZ_WIDGET* gui = nullptr;
    WZ_SKIN_THEME skin_theme;

    // --- Estado e Controle ---
    GameState state;
    float timeSinceLastPipe;
    ALLEGRO_BITMAP* currentPipeTexture;

    // --- Gerador de Números Aleatórios ---
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    // --- Tema ---
    const Theme& selectedTheme;

    // --- Métodos de Lógica Interna ---
    void updatePlaying(float deltaTime);
    void checkCollisions();
    void spawnPipe();
    void initiateDeathSequence();
    void restart();
    void initGUI();

public:
    /**
     * @brief Construtor da GameScene que aceita um objeto Theme.
     * @param sceneManager Ponteiro para o gerenciador de cenas.
     * @param selectedTheme O tema escolhido pelo jogador, passado por referência constante.
     */
    GameScene(SceneManager* sceneManager, const Theme& selectedTheme);
    ~GameScene();

    // --- Implementação dos contratos de Scene, IDrawable, IUpdatable ---
    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void draw() const override;
};