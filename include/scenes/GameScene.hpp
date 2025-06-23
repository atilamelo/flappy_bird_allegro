/**
 * @file GameScene.hpp
 * @brief Definição da cena principal do jogo, que gerencia o ciclo de vida e a interação dos objetos de jogo.
 * @version 3.0
 * @date 2025-06-22
 */
#pragma once

#include "core/Scene.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "actors/Floor.hpp"
#include "actors/PipePool.hpp"
#include "actors/effects/SplashScreen.hpp"
#include "actors/ui/GameOverScreen.hpp"
#include "actors/ui/GetReadyUI.hpp"
#include "managers/ScoreManager.hpp"
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
 * @brief Orquestra todo o gameplay, gerenciando os objetos de jogo através de interfaces.
 *
 * A GameScene é responsável por criar, atualizar, desenhar e destruir todos os
 * elementos do jogo. Ela utiliza uma máquina de estados para controlar o fluxo
 * e listas polimórficas para processar os objetos de forma desacoplada.
 */
class GameScene : public Scene
{
private:
    // --- Propriedade dos Objetos Principais ---
    // Mantemos a propriedade dos objetos aqui para garantir seu tempo de vida.
    std::unique_ptr<Bird> bird;
    std::unique_ptr<ParallaxBackground> background;
    std::unique_ptr<Floor> floor;
    std::unique_ptr<SplashScreen> flashEffect;
    std::unique_ptr<GameOverScreen> gameOverScreen;
    std::unique_ptr<GetReadyUI> getReadyUI;
 
    PipePool pipePool;
    ScoreManager scoreManager;
    
    // --- Listas de Observadores (Não-proprietárias) para Polimorfismo ---
    // Essas listas contêm ponteiros para os objetos que precisam ser atualizados ou desenhados.
    std::vector<IUpdatable*> updatables;
    std::vector<IDrawable*> drawables;

    // --- Estado e Controle ---
    GameState state;
    float timeSinceLastPipe = 0.0f;

    // --- Gerador de Números Aleatórios ---
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    // --- Métodos de Lógica Interna ---
    void buildEntityLists();
    void updatePlaying(float deltaTime);
    void checkCollisions();
    void spawnPipe();
    void initiateDeathSequence();
    void restart();

public:
    /**
     * @brief Construtor da GameScene.
     * @param sceneManager Ponteiro para o gerenciador de cenas.
     */
    GameScene(SceneManager* sceneManager);

    /**
     * @brief Processa eventos de input do usuário, controlando as transições de estado.
     * @param event O evento do Allegro a ser processado.
     */
    void processEvent(const ALLEGRO_EVENT& event) override;

    /**
     * @brief Atualiza todos os objetos de jogo atualizáveis na cena.
     * @param deltaTime O tempo decorrido desde o último frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Desenha todos os objetos de jogo desenháveis na tela, respeitando a ordem das camadas.
     */
    void draw() override;
};