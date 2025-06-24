/**
 * @file StartMenu.cpp
 * @brief Implementação dos métodos da cena do menu principal.
 */
#include "scenes/RankingScene.hpp"
#include "scenes/StartMenu.hpp"
#include "scenes/GameScene.hpp"
#include "managers/SceneManager.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include "scenes/CharacterSelectionScene.hpp"
#include "util/Theme.hpp"
#include "core/PlayerData.hpp"
#include <iostream>
#include <allegro5/allegro_image.h>

StartMenu::StartMenu(SceneManager *sceneManager)
    : Scene(sceneManager)
{
    buildUI();
}

StartMenu::~StartMenu()
{
    std::cout << "Destruindo a GUI do StartMenu." << std::endl;
    if (gui)
    {
        wz_destroy(gui);
    }
}

void StartMenu::buildUI()
{
    ResourceManager &rm = ResourceManager::getInstance();

    // 1. POSICIONAMENTO DA LOGO
    std::vector<ALLEGRO_BITMAP *> bird_frames = {
        rm.getBitmap("yellowbird-downflap"), rm.getBitmap("yellowbird-midflap"), rm.getBitmap("yellowbird-upflap")};

    float logo_w = al_get_bitmap_width(rm.getBitmap("logo_text")) * 2.0f;
    float logo_h = al_get_bitmap_height(rm.getBitmap("logo_text")) * 2.0f;
    float total_visual_width = logo_w + 10.0f + BIRD_WIDTH;
    float logo_x = (BUFFER_W - total_visual_width) / 2.0f;
    float logo_y = 50.0f;

    flappyLogo = std::make_unique<FlappyLogo>(logo_x, logo_y, logo_w, logo_h, rm.getBitmap("logo_text"), bird_frames);
    background_image = rm.getBitmap("bg_day");

    // 2. CONFIGURAÇÃO DA GUI (WIDGETZ)
    memset(&skin_theme, 0, sizeof(skin_theme));
    memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    font = al_create_builtin_font();
    skin_theme.theme.font = font;
    skin_theme.theme.color1 = al_map_rgba_f(0.97, 0.97, 0.96, 1);
    skin_theme.theme.color2 = al_map_rgba_f(0.98, 0.53, 0.07, 1);
    skin_theme.editbox_bitmap = al_load_bitmap("data/editbox.png");

    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME *)&skin_theme);

    // --- ESTRUTURA DO LAYOUT ---

    float h_spacing = 10.0f;
    float v_spacing = 15.0f;

    // Dimensões originais dos botões
    float play_button_w = 52;
    float rank_button_w = 52;

    // Calcula a largura total necessária para a fileira de botões
    float total_buttons_width = play_button_w + rank_button_w + (3 * h_spacing);

    // <<< CALCULA A POSIÇÃO X INICIAL PARA CENTRALIZAR O LAYOUT >>>
    float layout_x = (BUFFER_W - total_buttons_width) / 2.0f;

    wz_create_fill_layout(gui, layout_x, 180, total_buttons_width, 0, h_spacing, v_spacing, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 0);

    // Caixa de texto
    editbox = wz_create_editbox(gui, 0, 0, 150, 30, al_ustr_new(""), 1, 10);

    // Botões adicionados ao layout.
    wz_create_image_button(gui, 0, 0, 52, 29, rm.getBitmap("btn_start_normal"), rm.getBitmap("btn_start_pressed"), rm.getBitmap("btn_start_focused"), rm.getBitmap("btn_start_focused"), 11);
    wz_create_image_button(gui, 0, 0, 52, 29, rm.getBitmap("btn_rank_normal"), rm.getBitmap("btn_rank_pressed"), rm.getBitmap("btn_rank_focused"), rm.getBitmap("btn_start_focused"), 12);
    wz_create_image_button(gui, 0, 0, 40, 14, rm.getBitmap("btn_quit_normal"), rm.getBitmap("btn_quit_pressed"), rm.getBitmap("btn_quit_focused"), rm.getBitmap("btn_start_focused"), 13);

    // Registrar fonte dos eventos
    ALLEGRO_EVENT_QUEUE *queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}

void StartMenu::processEvent(const ALLEGRO_EVENT &event)
{
    // Validação de Input para a caixa de texto
    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        int key_char = event.keyboard.unichar;
        int keycode = event.keyboard.keycode;
        std::cout << keycode << std::endl;
        bool is_valid = (key_char >= 'a' && key_char <= 'z') ||
                        (key_char >= 'A' && key_char <= 'Z') ||
                        (key_char >= '0' && key_char <= '9') ||
                        (key_char == ' ') ||
                        (keycode == ALLEGRO_KEY_BACKSPACE) ||
                        (keycode == ALLEGRO_KEY_DELETE) ||
                        (keycode == ALLEGRO_KEY_LEFT) ||
                        (keycode == ALLEGRO_KEY_RIGHT) ||
                        (keycode == ALLEGRO_KEY_HOME) ||
                        (keycode == ALLEGRO_KEY_END) ||
                        (keycode == ALLEGRO_KEY_UP) ||
                        (keycode == ALLEGRO_KEY_DOWN);
        std::cout << "isvalid: " << is_valid << std::endl;
        if (!is_valid)
        {
            return; // Ignora o caractere inválido
        }
    }

    ALLEGRO_EVENT e = event;
    wz_send_event(gui, &e);

    if (e.type == WZ_BUTTON_PRESSED)
    {
        int button_id = (int)e.user.data1;
        if (button_id == 11)
        { // Iniciar
            if (editbox->text->slen >= 3 && editbox->text->slen <= 20)
            {              
                PlayerData::getInstance().setName(reinterpret_cast<char*>(editbox->text->data));
                sceneManager->setCurrentScene(std::make_unique<CharacterSelectionScene>(sceneManager));
            }
            else
            {
                std::cout << "Nome de usuário inválido. Deve ter entre 3 e 20 caracteres." << std::endl;
            }
        }
        else if (button_id == 12)
        { // Ranking
        sceneManager->setCurrentScene(std::make_unique<RankingScene>(sceneManager));
        }
        else if (button_id == 13)
        { // Sair
            sceneManager->shutdown();
        }
    }
}

void StartMenu::update(float deltaTime)
{
    wz_update(gui, deltaTime);
    if (flappyLogo)
    {
        flappyLogo->update(deltaTime);
    }
}

void StartMenu::draw() const
{
    // 1. Desenha o fundo primeiro
    if (background_image)
    {
        al_draw_bitmap(background_image, 0, 0, 0);
    }

    // 2. Desenha a UI da WidgetZ (botões e editbox)
    if (gui)
    {
        wz_draw(gui);
    }

    // 3. Desenha o logo animado por cima de tudo
    if (flappyLogo)
    {
        flappyLogo->draw();
    }
}