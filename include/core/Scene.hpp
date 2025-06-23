/**
 * @file Scene.hpp
 * @brief Definição da classe base abstrata para todas as cenas do jogo.
 */
#pragma once

#include <allegro5/allegro.h>

// Forward declaration para evitar dependência circular
class SceneManager;

/**
 * @class Scene
 * @brief Classe base abstrata para uma cena do jogo (ex: menu, fase, etc.).
 *
 * Define o contrato que todas as cenas devem seguir, incluindo métodos para
 * processar eventos, atualizar a lógica e desenhar na tela.
 */
class Scene
{
protected:
    SceneManager* sceneManager;

public:
    /**
     * @brief Construtor da cena.
     * @param sceneManager Ponteiro para o gerenciador que controla esta cena.
     */
    Scene(SceneManager* sceneManager);

    /**
     * @brief Destrutor virtual.
     * @note Essencial para garantir que os destrutores das classes filhas sejam chamados.
     */
    virtual ~Scene();

    // --- Métodos Virtuais Puros (Contrato) ---
    virtual void processEvent(const ALLEGRO_EVENT& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
};