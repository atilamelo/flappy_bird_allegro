/**
 * @file Game.hpp
 * @brief Definição da classe principal Game, que encapsula toda a aplicação.
 * @version 1.0
 * @date 2025-06-22
 */
#pragma once

#include "managers/SceneManager.hpp"
#include <allegro5/allegro.h>
#include <memory>

/**
 * @class Game
 * @brief A classe principal que gerencia o ciclo de vida da aplicação.
 *
 * Esta classe é responsável por inicializar o Allegro, gerenciar os recursos
 * principais (display, timer, fila de eventos), e executar o loop principal do jogo.
 * Ela utiliza o padrão RAII para garantir que todos os recursos sejam liberados
 * corretamente em seu destrutor.
 */
class Game {
private:
    // --- Recursos do Allegro ---
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;

    // --- Gerenciadores ---
    SceneManager sceneManager;
    
    // --- Controle do Loop ---
    bool isRunning;

    /**
     * @brief Inicializa todos os componentes do Allegro e recursos do jogo.
     */
    void initialize();

    /**
     * @brief Processa um único evento da fila do Allegro.
     * @param event O evento a ser processado.
     */
    void processEvent(const ALLEGRO_EVENT& event);

    /**
     * @brief Atualiza a lógica do jogo com base no tempo decorrido.
     * @param deltaTime O tempo, em segundos, desde a última atualização.
     */
    void update(float deltaTime);

    /**
     * @brief Desenha o frame atual na tela.
     */
    void draw();

    /**
     * @brief Realiza a limpeza de todos os recursos alocados.
     */
    void shutdown();

public:
    /**
     * @brief Construtor da classe Game.
     *
     * Chama o método de inicialização para configurar a aplicação.
     */
    Game();

    /**
     * @brief Destrutor da classe Game.
     *
     * Garante que o método shutdown seja chamado para liberar todos os recursos.
     */
    ~Game();

    /**
     * @brief Inicia e executa o loop principal do jogo.
     */
    void run();
};