/**
 * @file GameScene.hpp
 * @brief Definição da cena principal do jogo, que gerencia todo o gameplay.
 * @version 2.0
 * @date 2025-06-22
 */
#pragma once

#include "Scene.hpp"
#include "actors/Bird.hpp"
#include "actors/ParallaxBackground.hpp"
#include "actors/Floor.hpp"
#include "util/PipePool.hpp"
#include "ScoreManager.hpp"
#include <memory>
#include <random>

enum class GameState {
    GAME_INIT,      // Jogo na tela inicial, esperando o jogador.
    PLAYING,        // Jogo em andamento.
    DYING,          // Animação de morte do pássaro em andamento.
    GAME_OVER       // Tela de "Fim de Jogo", aguardando reinício.
};

class GameScene : public Scene
{
private:
    // --- Membros de Jogo ---
    std::unique_ptr<Bird> bird;
    std::unique_ptr<ParallaxBackground> background;
    std::unique_ptr<Floor> floor;
    PipePool pipePool;
    ScoreManager scoreManager;
    
    // --- Estado e Controle ---
    GameState state;
    float timeSinceLastPipe = 0.0f;

    // --- Gerador de Números Aleatórios ---
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    // --- Métodos de Lógica Interna ---
    void updatePlaying(float deltaTime);
    void updateDying(float deltaTime);
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
     * @brief Processa eventos de input do usuário.
     * @param event O evento do Allegro a ser processado.
     */
    void processEvent(const ALLEGRO_EVENT& event) override;

    /**
     * @brief Atualiza a lógica da cena a cada frame.
     * @param deltaTime O tempo decorrido desde o último frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Desenha todos os elementos da cena na tela.
     */
    void draw() override;
};