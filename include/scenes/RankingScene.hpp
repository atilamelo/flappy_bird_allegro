/**
 * @file RankingScene.hpp
 * @brief Definição da cena de ranking de pontuações.
 */
#pragma once

#include "core/Scene.hpp"
#include "widgetz/widgetz.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <vector>
#include <string>
#include <memory>

class SceneManager;

// STRUCT PARA ARMAZENAR DADOS DE EXIBIÇÃO DO PLACAR
class RankingScene : public Scene {
private:
    // --- Recursos Visuais ---
    ALLEGRO_BITMAP* background_image = nullptr;
    ALLEGRO_BITMAP* title_image = nullptr;
    ALLEGRO_BITMAP* scoreboard_image = nullptr;
    ALLEGRO_FONT* text_font = nullptr;

    // --- Dados do Ranking ---
    std::vector<std::pair<std::string, int>> scores;
    int currentPage = 0;
    const int scoresPerPage = 5;

    // --- UI com WidgetZ ---
    WZ_WIDGET* gui = nullptr;
    WZ_SKIN_THEME skin_theme;

    void buildUI();

    void loadDummyData();
    
    void drawScores() const;

public:
    RankingScene(SceneManager* sceneManager);
    ~RankingScene();

    void processEvent(const ALLEGRO_EVENT& event) override;
    void update(float deltaTime) override;
    void draw() const override;
};