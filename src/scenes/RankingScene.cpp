#include "scenes/RankingScene.hpp"
#include "managers/SceneManager.hpp"
#include "managers/ResourceManager.hpp"
#include "scenes/StartMenu.hpp"
#include "Constants.hpp"
#include "util/ScoreSystem.hpp"
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <iostream>

RankingScene::RankingScene(SceneManager* sceneManager)
    : Scene(sceneManager), currentPage(0) {
    loadDummyData();
    buildUI();
}

RankingScene::~RankingScene() {
    if (gui) {
    std::cout << "Destruindo a GUI do Ranking." << std::endl;
        wz_destroy(gui);
    }
    if (text_font) {
        al_destroy_font(text_font);
    }
}

void RankingScene::loadDummyData() {
    ScoreSystem& scoreSy = ScoreSystem::getInstance();
    scores = scoreSy.getTopScores(20);
}

void RankingScene::buildUI() {
    ResourceManager& rm = ResourceManager::getInstance();

    // Carregar recursos visuais
    background_image = rm.getBitmap("background-day");
    title_image = rm.getBitmap("logo_text");
    scoreboard_image = rm.getBitmap("ranking_box");
    text_font = al_create_builtin_font();

    // Configuração da GUI (WidgetZ)
    memset(&skin_theme, 0, sizeof(skin_theme));
    memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    skin_theme.theme.font = text_font;
    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME*)&skin_theme);

    // --- Layout dos Botões ---
    float button_w = 80.0f;
    float button_h = 28.0f;
    float spacing = 15.0f;
    
    // Posição Y abaixo do placar
    float scoreboard_h = al_get_bitmap_height(scoreboard_image);
    float buttons_y = (BUFFER_H - scoreboard_h) / 2.0f + scoreboard_h + 20;

    // Botões de Paginação (Anterior e Próximo)
    float page_buttons_total_width = button_w * 2 + spacing;
    float page_buttons_start_x = (BUFFER_W - page_buttons_total_width) / 2.0f;
    
    wz_create_image_button(gui, page_buttons_start_x, buttons_y, button_w, button_h, rm.getBitmap("quit_button"), rm.getBitmap("quit_button_pressed"), rm.getBitmap("quit_button_focused"), nullptr, 1); // Voltar Página
    wz_create_image_button(gui, page_buttons_start_x + button_w + spacing, buttons_y, button_w, button_h, rm.getBitmap("quit_button"), rm.getBitmap("quit_button_pressed"), rm.getBitmap("quit_button_focused"), nullptr, 2); // Avançar Página

    // Botão de Voltar para o Menu
    float back_button_x = (BUFFER_W - button_w) / 2.0f;
    float back_button_y = buttons_y + button_h + spacing;
    wz_create_image_button(gui, back_button_x, back_button_y, button_w, button_h, rm.getBitmap("menu_button"), rm.getBitmap("menu_button_pressed"), rm.getBitmap("menu_button_focused"), nullptr, 3); // Voltar ao menu

    ALLEGRO_EVENT_QUEUE* queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}

void RankingScene::processEvent(const ALLEGRO_EVENT& event) {
    ALLEGRO_EVENT e = event;
    wz_send_event(gui, &e);

    if (e.type == WZ_BUTTON_PRESSED) {
        int button_id = (int)e.user.data1;
        switch (button_id) {
            case 1: // Voltar Página
                if (currentPage > 0) {
                    currentPage--;
                }
                break;
            case 2: // Avançar Página
                if ((currentPage + 1) * scoresPerPage < scores.size()) {
                    currentPage++;
                }
                break;
            case 3: // Voltar ao Menu
                sceneManager->setCurrentScene(std::make_unique<StartMenu>(sceneManager));
                break;
        }
    }
}

void RankingScene::update(float deltaTime) {
    wz_update(gui, deltaTime);
}

void RankingScene::draw() const {
    float title_x;
    // 1. Desenha o fundo
    if (background_image) {
        al_draw_bitmap(background_image, 0, 0, 0);
    }

    // 2. Desenha o título
    if (title_image) {
        title_x = (BUFFER_W - al_get_bitmap_width(title_image) * 2.0f) / 2.0f;
        al_draw_scaled_bitmap(title_image, 0, 0, al_get_bitmap_width(title_image), al_get_bitmap_height(title_image), title_x, 50, al_get_bitmap_width(title_image) * 2.0f, al_get_bitmap_height(title_image) * 2.0f, 0);
    }

    // 3. Desenha o placar
    if (scoreboard_image) {
        float board_x = (BUFFER_W - al_get_bitmap_width(scoreboard_image)) / 2.0f;
        float board_y = (title_x*3);
        al_draw_bitmap(scoreboard_image, board_x, board_y, 0);
        
        // 4. Desenha as pontuações dentro do placar
        drawScores();
    }
    
    // 5. Desenha a UI (botões)
    if (gui) {
        wz_draw(gui);
    }
}

void RankingScene::drawScores() const {
    if (!text_font) return;
    
    // Posições relativas ao placar
    float board_x = (BUFFER_W - al_get_bitmap_width(scoreboard_image)) / 2.0f;
    float board_y = ((BUFFER_W - al_get_bitmap_width(title_image) * 2.0f) / 2.0f)*3;
    
    // Posição inicial para o primeiro texto dentro do placar
    float start_y = board_y + 13;
    float line_height = 20;

    int start_index = currentPage * scoresPerPage;
    int end_index = std::min(start_index + scoresPerPage, (int)scores.size());

    for (int i = start_index; i < end_index; ++i) {
        const auto& playerScore = scores[i];
        float current_y = start_y + (i - start_index) * line_height;
        
        // Colunas para alinhar o texto
        float rank_x = board_x + 30;
        float name_x = board_x + 60;
        float score_x = board_x + 200;

        // Desenha o texto
        // Rank
        al_draw_textf(text_font, al_map_rgb(255, 255, 255), rank_x, current_y, ALLEGRO_ALIGN_LEFT, "%d.", i + 1);
        // Nome
        al_draw_text(text_font, al_map_rgb(255, 255, 255), name_x, current_y, ALLEGRO_ALIGN_LEFT, playerScore.first.c_str());
        // Pontuação
        al_draw_textf(text_font, al_map_rgb(255, 255, 255), score_x, current_y, ALLEGRO_ALIGN_RIGHT, "%d", playerScore.second);
    }
}