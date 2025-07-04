/**
 * @file StartMenu.cpp
 * @brief Implementação dos métodos de cena do menu principal.
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
#include "util/ScoreSystem.hpp"
#include <iostream>
#include <allegro5/allegro_image.h>

/**
 * @class StartMenu
 * @brief Classe responsável pela cena de menu inicial do jogo.
 *
 * Essa cena contém a interface gráfica com o botão de iniciar, ranking, sair, e um campo de texto
 * para inserção do nome do jogador. Também exibe o logo animado do jogo.
 */
 
/**
 * @brief Construtor da classe StartMenu.
 * 
 * Inicializa a cena com os elementos da interface do menu.
 * 
 * @param sceneManager Ponteiro para o gerenciador de cenas.
 */
StartMenu::StartMenu(SceneManager *sceneManager)
    : Scene(sceneManager)
{
    buildUI();
}

/**
 * @brief Destrutor da classe StartMenu.
 * 
 * Libera os recursos alocados para a GUI e imprime mensagem de depuração.
 */
StartMenu::~StartMenu()
{
    std::cout << "Destruindo a GUI do StartMenu." << std::endl;
    if (gui)
    {
        wz_destroy(gui);
    }
}

/**
 * @brief Constrói a interface gráfica do menu.
 * 
 * Essa função monta os elementos da interface gráfica, incluindo:
 * - Logo animado do jogo
 * - Botões (Iniciar, Ranking, Sair)
 * - Campo de texto para inserção do nome
 * 
 * Também registra os eventos na fila de eventos do Allegro.
 */
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
    background_image = rm.getBitmap("background-day");

    // 2. CONFIGURAÇÃO DA GUI (WIDGETZ)
    memset(&skin_theme, 0, sizeof(skin_theme));
    memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    font = al_create_builtin_font();
    skin_theme.theme.font = font;
    skin_theme.theme.color1 = al_map_rgba_f(0.97, 0.97, 0.96, 1);
    skin_theme.theme.color2 = al_map_rgba_f(0.98, 0.53, 0.07, 1);
    skin_theme.editbox_bitmap = rm.getBitmap("editbox");

    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME *)&skin_theme);

    // Layout dos elementos da interface
    float h_spacing = 10.0f;
    float v_spacing = 15.0f;

    float play_button_w = 52;
    float rank_button_w = 52;
    float total_buttons_width = play_button_w + rank_button_w + (3 * h_spacing);
    float layout_x = (BUFFER_W - total_buttons_width) / 2.0f;

    wz_create_fill_layout(gui, layout_x, 180, total_buttons_width, 0, h_spacing, v_spacing, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 0);

    // Caixa de entrada de nome do jogador
    editbox = wz_create_editbox(gui, 0, 0, 150, 30, al_ustr_new(""), 1, 10);

    // Botões adicionados ao layout.
    wz_create_image_button(gui, 0, 0, 52, 29, rm.getBitmap("home_start_button"), rm.getBitmap("home_start_button_pressed"), rm.getBitmap("home_start_button_focused"), rm.getBitmap("home_start_button_focused"), 11);
    wz_create_image_button(gui, 0, 0, 52, 29, rm.getBitmap("rank_button"), rm.getBitmap("rank_button_pressed"), rm.getBitmap("rank_button_focused"), rm.getBitmap("home_start_button_focused"), 12);
    wz_create_image_button(gui, 0, 0, 40, 14, rm.getBitmap("quit_button"), rm.getBitmap("quit_button_pressed"), rm.getBitmap("quit_button_focused"), rm.getBitmap("home_start_button_focused"), 13);

    // Registrar eventos
    ALLEGRO_EVENT_QUEUE *queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}

/**
 * @brief Processa os eventos recebidos do sistema.
 * 
 * Responsável por:
 * - Filtrar entradas inválidas no campo de nome
 * - Lidar com cliques nos botões (iniciar, ranking, sair)
 * 
 * @param event Evento capturado pela fila de eventos do Allegro.
 */
void StartMenu::processEvent(const ALLEGRO_EVENT &event)
{
    // Validação de caracteres permitidos no campo de texto
    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        int key_char = event.keyboard.unichar;
        int keycode = event.keyboard.keycode;
        std::cout << keycode << std::endl;
        bool is_valid = (key_char >= 'a' && key_char <= 'z') ||
                        (key_char >= 'A' && key_char <= 'Z') ||
                        (key_char >= '0' && key_char <= '9') ||
                        (keycode == ALLEGRO_KEY_DELETE) ||
                        (keycode == ALLEGRO_KEY_BACKSPACE) ||
                        (keycode == ALLEGRO_KEY_LEFT) ||
                        (keycode == ALLEGRO_KEY_RIGHT) ||
                        (keycode == ALLEGRO_KEY_HOME) ||
                        (keycode == ALLEGRO_KEY_END) ||
                        (keycode == ALLEGRO_KEY_UP) ||
                        (keycode == ALLEGRO_KEY_DOWN);
        std::cout << "isvalid: " << is_valid << std::endl;
        if (!is_valid)
        {
            return;
        }
        
        // Validação de tamanho máximo da string na editbox
        std::string aux = (reinterpret_cast<char*>(editbox->text->data));
        if (((key_char >= 'a' && key_char <= 'z') ||
            (key_char >= 'A' && key_char <= 'Z') ||
            (key_char >= '0' && key_char <= '9')) && aux.length()>11)
        {
            return;
        }
    }

    ALLEGRO_EVENT e = event;
    wz_send_event(gui, &e);

    // Ações dos botões
    if (e.type == WZ_BUTTON_PRESSED)
    {
        int button_id = (int)e.user.data1;
        if (button_id == 11)
        {
            // Validação do texto recebido no input
            if (editbox->text->slen >= 3 && editbox->text->slen <= 12)
            {
                ScoreSystem& scoreSystem = ScoreSystem::getInstance();
                // Normalização do texto e conversão para string
                std::string aux = (reinterpret_cast<char*>(editbox->text->data));
                aux = scoreSystem.trim(aux);
                aux = scoreSystem.toUpper(aux);
                std::cout << aux << std::endl;
                // Armazenamento na PlayerData e chamada da próxima Scene
                PlayerData::getInstance().setName(aux);
                PlayerData::getInstance().setGames(0);
                sceneManager->setCurrentScene(std::make_unique<CharacterSelectionScene>(sceneManager));
            }
            else
            {
                std::cout << "Nome de usuário inválido. Deve ter entre 3 e 12 caracteres." << std::endl;
            }
        }
        else if (button_id == 12)
        {
            sceneManager->setCurrentScene(std::make_unique<RankingScene>(sceneManager));
        }
        else if (button_id == 13)
        {
            sceneManager->shutdown();
        }
    }
}

/**
 * @brief Atualiza os elementos da cena.
 * 
 * Atualiza o estado da GUI e animações da logo.
 * 
 * @param deltaTime Tempo decorrido desde o último frame.
 */
void StartMenu::update(float deltaTime)
{
    wz_update(gui, deltaTime);
    if (flappyLogo)
    {
        flappyLogo->update(deltaTime);
    }
}

/**
 * @brief Desenha os elementos da cena na tela.
 * 
 * A ordem de desenho é:
 * 1. Fundo
 * 2. Interface gráfica (editbox, botões)
 * 3. Logo animado
 */
void StartMenu::draw() const
{
    if (background_image)
    {
        al_draw_bitmap(background_image, 0, 0, 0);
    }

    if (gui)
    {
        wz_draw(gui);
    }

    if (flappyLogo)
    {
        flappyLogo->draw();
    }
}
