/**
 * @file StartMenu.hpp
 * @brief Definição da cena do menu inicial do jogo.
 */
#pragma once

#include "core/Scene.hpp"
#include "widgetz/widgetz.h"
#include "actors/menu/FlappyLogo.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <memory>

class SceneManager;

/**
 * @class StartMenu
 * @brief Gerencia e exibe a tela de menu principal.
 *
 * Esta cena renderiza os elementos estáticos (fundo, logo) diretamente com
 * Allegro e utiliza a biblioteca WidgetZ para os componentes interativos.
 */
class StartMenu : public Scene
{
private:
    // --- Entidades e Recursos ---
    std::unique_ptr<FlappyLogo> flappyLogo; ///< Logo animado.
    ALLEGRO_BITMAP* background_image = nullptr; ///< Ponteiro para o bitmap de fundo.

    // --- Membros da Biblioteca WidgetZ ---
    WZ_WIDGET* gui = nullptr;
    WZ_EDITBOX* editbox = nullptr;
    WZ_SKIN_THEME skin_theme;
    ALLEGRO_FONT* font = nullptr;

    /**
     * @brief Constrói a interface da cena buscando os recursos no ResourceManager.
     */
    void buildUI();

public:
    /**
     * @brief Construtor da cena do menu.
     * @param sceneManager Ponteiro para o gerenciador de cenas principal.
     */
    StartMenu(SceneManager* sceneManager);

    /**
     * @brief Destrutor da cena do menu.
     */
    ~StartMenu();

    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void draw() const override;
};