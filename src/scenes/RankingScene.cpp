#include "scenes/RankingScene.hpp"
#include "managers/SceneManager.hpp"
#include "managers/ResourceManager.hpp"
#include "scenes/StartMenu.hpp"
#include "Constants.hpp"
#include "util/ScoreSystem.hpp"
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <iostream>

/**
 * @brief Construtor da cena de Ranking.
 * @details Assim que a cena é criada, ela já carrega as pontuações e constrói a interface.
 */
RankingScene::RankingScene(SceneManager* sceneManager)
    : Scene(sceneManager), currentPage(0) {
    loadDummyData(); // Puxa os scores do sistema.
    buildUI();       // Monta os botões e a aparência da tela.
}

/**
 * @brief Destrutor da cena de Ranking.
 * @details Limpa os recursos que essa cena criou, como a GUI e a fonte.
 * É importante pra não deixar lixo na memória.
 */
RankingScene::~RankingScene() {
    if (gui) {
        std::cout << "Destruindo a GUI do Ranking." << std::endl;
        wz_destroy(gui); // Libera a memória usada pela interface de botões.
    }
    if (text_font) {
        al_destroy_font(text_font); // Libera a memória da fonte.
    }
}

/**
 * @brief Carrega os dados das pontuações para exibir no ranking.
 */
void RankingScene::loadDummyData() {
    ScoreSystem& scoreSy = ScoreSystem::getInstance();
    // Pede ao ScoreSystem a lista com as melhores pontuações.
    scores = scoreSy.getTopScores(20);
}

/**
 * @brief Constrói todos os elementos da interface gráfica (UI).
 * @details Carrega as imagens, configura a biblioteca de GUI (WidgetZ) e cria os botões.
 */
void RankingScene::buildUI() {
    ResourceManager& rm = ResourceManager::getInstance();

    // Carregar recursos visuais
    background_image = rm.getBitmap("background-day");
    title_image = rm.getBitmap("logo_text");
    scoreboard_image = rm.getBitmap("ranking_box");
    text_font = al_create_builtin_font(); // Usa uma fonte padrão do Allegro pro texto.

    // --- Configuração da biblioteca de GUI (WidgetZ) ---
    // A gente copia um tema padrão e só troca a fonte, pra não começar do zero.
    memset(&skin_theme, 0, sizeof(skin_theme));
    memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    skin_theme.theme.font = text_font;
    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1); // Cria o "container" principal da nossa GUI.
    wz_set_theme(gui, (WZ_THEME*)&skin_theme);

    // --- Posicionamento dos Botões ---
    float button_w = 80.0f;
    float button_h = 28.0f;
    float spacing = 15.0f;
    
    // Calcula a posição Y pra ficar logo abaixo do placar.
    float scoreboard_h = al_get_bitmap_height(scoreboard_image);
    float buttons_y = (BUFFER_H - scoreboard_h) / 2.0f + scoreboard_h + 20;

    // Centraliza os botões de paginação (Anterior/Próximo) na tela.
    float page_buttons_total_width = button_w * 2 + spacing;
    float page_buttons_start_x = (BUFFER_W - page_buttons_total_width) / 2.0f;
    
    // Cria os botões e associa um ID a cada um (1, 2, 3...).
    // TODO: Usar imagens de setas em vez dos botões de "quit".
    wz_create_image_button(gui, page_buttons_start_x, buttons_y, button_w, button_h, rm.getBitmap("quit_button"), rm.getBitmap("quit_button_pressed"), rm.getBitmap("quit_button_focused"), nullptr, 1); // Voltar Página
    wz_create_image_button(gui, page_buttons_start_x + button_w + spacing, buttons_y, button_w, button_h, rm.getBitmap("quit_button"), rm.getBitmap("quit_button_pressed"), rm.getBitmap("quit_button_focused"), nullptr, 2); // Avançar Página

    // Botão de Voltar para o Menu
    float back_button_x = (BUFFER_W - button_w) / 2.0f;
    float back_button_y = buttons_y + button_h + spacing;
    wz_create_image_button(gui, back_button_x, back_button_y, button_w, button_h, rm.getBitmap("menu_button"), rm.getBitmap("menu_button_pressed"), rm.getBitmap("menu_button_focused"), nullptr, 3); // Voltar ao menu

    // Diz pra GUI escutar os eventos da fila principal do jogo.
    ALLEGRO_EVENT_QUEUE* queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}

/**
 * @brief Processa os eventos de input (mouse, teclado).
 */
void RankingScene::processEvent(const ALLEGRO_EVENT& event) {
    ALLEGRO_EVENT e = event;
    // Manda o evento pra biblioteca de GUI, pra ela ver se algum botão foi clicado.
    wz_send_event(gui, &e);

    // Se a GUI detectou que um botão foi apertado...
    if (e.type == WZ_BUTTON_PRESSED) {
        int button_id = (int)e.user.data1; // Pega o ID do botão que a gente definiu lá em cima.
        // E faz uma ação dependendo de qual botão foi.
        switch (button_id) {
            case 1: // Voltar Página
                if (currentPage > 0) {
                    currentPage--;
                }
                break;
            case 2: // Avançar Página
                // Só avança se tiver mais scores pra mostrar.
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

/**
 * @brief Atualiza a lógica da cena (nesse caso, só a da GUI).
 */
void RankingScene::update(float deltaTime) {
    wz_update(gui, deltaTime);
}

/**
 * @brief Desenha tudo da cena na tela.
 */
void RankingScene::draw() const {
    float title_x;
    // 1. Desenha o fundo.
    if (background_image) {
        al_draw_bitmap(background_image, 0, 0, 0);
    }

    // 2. Desenha o título.
    if (title_image) {
        title_x = (BUFFER_W - al_get_bitmap_width(title_image) * 2.0f) / 2.0f;
        al_draw_scaled_bitmap(title_image, 0, 0, al_get_bitmap_width(title_image), al_get_bitmap_height(title_image), title_x, 50, al_get_bitmap_width(title_image) * 2.0f, al_get_bitmap_height(title_image) * 2.0f, 0);
    }

    // 3. Desenha a imagem do placar.
    if (scoreboard_image) {
        float board_x = (BUFFER_W - al_get_bitmap_width(scoreboard_image)) / 2.0f;
        float board_y = (title_x*3);
        al_draw_bitmap(scoreboard_image, board_x, board_y, 0);
        
        // 4. Desenha as pontuações DENTRO do placar.
        drawScores();
    }
    
    // 5. Deixa a biblioteca de GUI desenhar os botões.
    if (gui) {
        wz_draw(gui);
    }
}

/**
 * @brief Desenha a lista de pontuações na tela.
 * @details Cuida da paginação e do alinhamento do texto.
 */
void RankingScene::drawScores() const {
    if (!text_font) return;
    
    // Calcula as posições com base na posição do placar, pra tudo ficar alinhado.
    float board_x = (BUFFER_W - al_get_bitmap_width(scoreboard_image)) / 2.0f;
    float board_y = ((BUFFER_W - al_get_bitmap_width(title_image) * 2.0f) / 2.0f)*3;
    
    float start_y = board_y + 13; // Posição Y da primeira linha de texto.
    float line_height = 20;       // Espaço entre uma linha e outra.

    // --- Lógica de Paginação ---
    // Define qual o primeiro e o último score a serem mostrados nessa página.
    int start_index = currentPage * scoresPerPage;
    int end_index = std::min(start_index + scoresPerPage, (int)scores.size());

    // Passa por cada pontuação que deve aparecer na página atual.
    for (int i = start_index; i < end_index; ++i) {
        const auto& playerScore = scores[i];
        float current_y = start_y + (i - start_index) * line_height;
        
        // Define as colunas pra alinhar o texto direitinho.
        float rank_x = board_x + 30;
        float name_x = board_x + 60;
        float score_x = board_x + 200;

        // Desenha as informações na tela.
        // Posição no ranking
        al_draw_textf(text_font, al_map_rgb(255, 255, 255), rank_x, current_y, ALLEGRO_ALIGN_LEFT, "%d.", i + 1);
        // Nome do jogador
        al_draw_text(text_font, al_map_rgb(255, 255, 255), name_x, current_y, ALLEGRO_ALIGN_LEFT, playerScore.first.c_str());
        // Pontuação
        al_draw_textf(text_font, al_map_rgb(255, 255, 255), score_x, current_y, ALLEGRO_ALIGN_RIGHT, "%d", playerScore.second);
    }
}