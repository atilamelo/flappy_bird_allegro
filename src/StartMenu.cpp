/**
 * @file StartMenu.cpp
 * @brief Implementação dos métodos da cena do menu principal.
 */
#include "StartMenu.hpp"
#include "core/GameScene.hpp"
#include "managers/SceneManager.hpp"
#include "Constants.hpp"

#include <iostream>
#include <allegro5/allegro_image.h>

StartMenu::StartMenu(SceneManager* sceneManager) 
    : Scene(sceneManager) {
    // Inicializa todos os recursos ao criar a cena.
    loadAssets();
}

StartMenu::~StartMenu() {
    std::cout << "Destruindo StartMenu e seus recursos." << std::endl;
    
    // Libera a memória alocada pela biblioteca WidgetZ.
    wz_destroy(gui);

    // Libera a memória dos bitmaps que carregamos manualmente.
    if (logo_image) {
        al_destroy_bitmap(logo_image);
    }
    if (background_image) {
        al_destroy_bitmap(background_image);
    }
}

void StartMenu::loadAssets() {
    const float fator_escala = 1.5f;

    al_init_image_addon();

    // 1. CARREGAMENTO DOS ASSETS MANUAIS (Fundo e Logo)
    background_image = al_load_bitmap("data/background-day.png");
    logo_image = al_load_bitmap("data/flappy_bird.png");
 
    if (logo_image) {
        logo_w = al_get_bitmap_width(logo_image) * 2.0f;
        logo_h = al_get_bitmap_height(logo_image) * 2.0f;
        logo_x = (BUFFER_W - logo_w) / 2;
        logo_y = 50.0f;
    }

    // 2. CONFIGURAÇÃO DA GUI E LAYOUT ÚNICO
    memset(&skin_theme, 0, sizeof(skin_theme));
    memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    font = al_create_builtin_font();
    skin_theme.theme.font = font;
    skin_theme.theme.color1 = al_map_rgba_f(0.37, 0.89, 0.44, 1);
    skin_theme.theme.color2 = al_map_rgba_f(0.99, 0.63, 0.27, 1);
	skin_theme.editbox_bitmap = al_load_bitmap("data/editbox.png");
	ALLEGRO_BITMAP* back= al_load_bitmap("data/background-day.png");

    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME*)&skin_theme);
    // --- ESTRUTURA DE LAYOUT ÚNICO E CENTRALIZADO ---

    float h_spacing = 10.0f;
    float v_spacing = 15.0f;

    // Dimensões originais dos botões
    float play_button_w = 52;
    float rank_button_w = 52;
    float quit_button_w = 40;
    
    // Calcula a largura total necessária para a fileira de botões
    float total_buttons_width = play_button_w + rank_button_w + quit_button_w + (2 * h_spacing);
    
    // <<< PASSO 1: CALCULA A POSIÇÃO X INICIAL PARA CENTRALIZAR O LAYOUT >>>
    float layout_x = (BUFFER_W - total_buttons_width) / 2.0f;
    
    // Cria um ÚNICO layout que irá organizar todos os widgets seguintes.
    // Agora, usamos a posição X calculada para posicionar o layout em si.
    wz_create_fill_layout(gui, layout_x, 180, total_buttons_width, 0, h_spacing, v_spacing, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 0);

        // A caixa de texto será centralizada dentro do layout.
        editbox = wz_create_editbox(gui, 0, 0, 150, 30, al_ustr_new(""), 1, 10);
        
        // A fileira de botões também será centralizada como um grupo.
        wz_create_image_button(gui, 0, 0, play_button_w, 29, al_load_bitmap("data/home_start_button.png"), al_load_bitmap("data/home_start_button_pressed.png"), al_load_bitmap("data/home_start_button_focused.png"), al_load_bitmap("data/home_start_button.png"), 11);
        wz_create_image_button(gui, 0, 0, rank_button_w, 29, al_load_bitmap("data/rank_button.png"), al_load_bitmap("data/rank_button_pressed.png"), al_load_bitmap("data/rank_button_focused.png"), al_load_bitmap("data/rank_button.png"), 12);
        wz_create_image_button(gui, 0, 0, quit_button_w, 14, al_load_bitmap("data/quit_button.png"), al_load_bitmap("data/quit_button_pressed.png"), al_load_bitmap("data/quit_button_focused.png"), al_load_bitmap("data/quit_button.png"), 13);

    // Registrar fontes de eventos
    ALLEGRO_EVENT_QUEUE* queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}


void StartMenu::draw() {
    // A ORDEM DE DESENHO É CRUCIAL PARA O EFEITO DE CAMADAS

    // 1. Desenha o fundo primeiro, para que ele fique atrás de tudo.
    if (background_image) {
        al_draw_bitmap(background_image, 0, 0, 0);
    }
    
    // 2. Desenha todos os widgets da GUI (botões, editbox) por cima do fundo.
    wz_draw(gui);
    
    // 3. Desenha o logo por último, para que ele fique na frente de tudo.
    if (logo_image) {
        al_draw_scaled_bitmap(logo_image,
            0, 0, al_get_bitmap_width(logo_image), al_get_bitmap_height(logo_image),
            logo_x, logo_y, logo_w, logo_h, 0
        );
    }
}
void StartMenu::processEvent(const ALLEGRO_EVENT& event) {
    ALLEGRO_EVENT e = event;
    wz_send_event(gui, &e);

    if (e.type == WZ_BUTTON_PRESSED) {
        int button_id = (int)e.user.data1;
        if (button_id == 11) { // Iniciar
            if (editbox->text->slen > 3 && editbox->text->slen < 20) {
                sceneManager->set_current_scene(std::make_unique<GameScene>(sceneManager));
            }
        } else if (button_id == 12) { // Ranking
            std::cout << "Placar!" << std::endl;
        } else if (button_id == 13) { // Sair
            sceneManager->shutdown();
        }
    }
}

void StartMenu::update(float deltaTime) {
    wz_update(gui, deltaTime);
}