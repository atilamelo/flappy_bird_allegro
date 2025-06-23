/**
 * @file CharacterSelectionScene.hpp
 * @brief Define a cena de seleção de temas/personagens.
 */
#pragma once

#include "core/Scene.hpp"
#include "interfaces/IDrawable.hpp"
#include "util/Theme.hpp"
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

/**
 * @class CharacterSelectionScene
 * @brief Permite ao jogador escolher um tema visual antes de iniciar o jogo.
 */
class CharacterSelectionScene : public Scene {
private:
    std::vector<Theme> themes;                 ///< Vetor de todos os temas disponíveis.
    std::vector<ALLEGRO_BITMAP*> preview_sprites; ///< Sprites para exibição na seleção.
    int selectedIndex;

    ALLEGRO_COLOR selection_color;
    ALLEGRO_FONT* font;

    std::vector<int> positionsX;

    /**
     * @brief Constrói os objetos de Tema a partir do ResourceManager.
     */
    void buildThemes();

public:
    CharacterSelectionScene(SceneManager* sceneManager);
    ~CharacterSelectionScene();

    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void draw() const override;
};