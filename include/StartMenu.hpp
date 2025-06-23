/**
 * @file StartMenu.hpp
 * @brief Definição da cena do menu inicial do jogo.
 * @version 3.0
 * @date 2025-06-22
 */
#pragma once

#include "core/Scene.hpp"
#include "widgetz/widgetz.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>

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
    // --- Recursos Gráficos Manuais ---
    ALLEGRO_BITMAP* background_image; ///< Bitmap da imagem de fundo.
    ALLEGRO_BITMAP* logo_image;       ///< Bitmap da imagem do logo.
    float logo_x, logo_y;             ///< Posição X e Y para desenhar o logo.
    float logo_w, logo_h;             ///< Largura e altura para desenhar o logo (escalado).

    // --- Membros da Biblioteca WidgetZ (Apenas para UI Interativa) ---
    WZ_WIDGET* gui;         ///< O widget raiz que contém os botões e editbox.
    WZ_EDITBOX* editbox;    ///< A caixa de texto para o nome do jogador.
    WZ_SKIN_THEME skin_theme; ///< O tema visual para os widgets.
    ALLEGRO_FONT* font;     ///< A fonte usada pela GUI.

    /**
     * @brief Carrega todos os assets visuais e constrói a interface da cena.
     */
    void loadAssets();

public:
    /**
     * @brief Construtor da cena do menu.
     * @param sceneManager Ponteiro para o gerenciador de cenas principal.
     */
    StartMenu(SceneManager* sceneManager);

    /**
     * @brief Destrutor da cena do menu.
     *
     * Responsável por liberar todos os recursos alocados.
     */
    ~StartMenu();

    // --- Métodos da Interface Scene ---
    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void draw() override;
};