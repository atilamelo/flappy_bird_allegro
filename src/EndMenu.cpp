#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include "widgetz/widgetz.h"
#include "EndMenu.hpp"
#include "Scene.hpp"
#include "Constants.hpp"
#include "SceneManager.hpp"
#include "GameScene.hpp"
#include "StartMenu.hpp"

EndMenu::EndMenu(SceneManager *sceneManager) 
    : Scene(sceneManager) {
    loadAssets();
}

EndMenu::~EndMenu() {
    // Clean up any resources if necessary
    std::cout << "Destruindo EndMenu" << std::endl;
    wz_destroy(gui);
}

void EndMenu::loadAssets() {
    memset(&theme, 0, sizeof(theme));
    memcpy(&theme, &wz_def_theme, sizeof(theme));
    font = al_create_builtin_font();
    theme.font = font;
    theme.color1 = al_map_rgba_f(0.37, 0.89, 0.44, 1);
    theme.color2 = al_map_rgba_f(0.99, 0.63, 0.27, 1);

    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME*)&theme);
    
    wz_create_fill_layout(gui, 0 * size, 0 * size, BUFFER_W, BUFFER_H, 0.8 * BUFFER_W, 0.1 * BUFFER_H, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 0);
        wz_create_textbox(gui, 0, 0, 200 * size, 10 * size, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("GAME OVER"), 1, -1);       
        wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("AGAIN"), 1, 1, 11);
        wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("NEW GAME"), 1, 1, 12);
        wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("SCORE"), 1, 1, 13);
        wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("QUIT"), 1, 1, 14);
    wz_create_layout_stop(gui, 0);

    // Register the event sources for WidgetZ
    ALLEGRO_EVENT_QUEUE* queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}

void EndMenu::processEvent(const ALLEGRO_EVENT& event) {
    //make a copy of the event to permit modification
    //this is necessary because WidgetZ modifies the event
    //to indicate that it has been handled
    ALLEGRO_EVENT e = event;

    wz_send_event(gui, &e);

    switch (e.type) {
        case WZ_BUTTON_PRESSED:
            std::cout << "WZ-BUTTON " << (int)e.user.data1 << std::endl;
            if ((int)e.user.data1 == 11) {
                std::cout << "Voltando ao jogo..." << std::endl;
                sceneManager->set_current_scene(std::make_unique<GameScene>(sceneManager));
            }
            if ((int)e.user.data1 == 12) {
                std::cout << "Reiniciando o jogo..." << std::endl;
                sceneManager->set_current_scene(std::make_unique<StartMenu>(sceneManager));
            }
            if ((int)e.user.data1 == 13) {
                std::cout << "Carregando placar..." << std::endl;
                std::cout << e.user.data1 << std::endl;
            }
            if ((int)e.user.data1 == 14) {
                std::cout << "Saindo..." << std::endl;
                sceneManager->shutdown();
            }
            break;
    }
}

void EndMenu::update(float deltaTime) {
    wz_update(gui, deltaTime);
}

void EndMenu::gameOver() {

}

void EndMenu::draw() {    
    wz_draw(gui);
}