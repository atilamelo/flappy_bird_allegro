#include "StartMenu.hpp"
#include "Scene.hpp"
#include "Constants.hpp"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include "widgetz/widgetz.h"

StartMenu::StartMenu(SceneManager *sceneManager) 
    : Scene(sceneManager) {
    // isWidgetZ=true;
    loadAssets();
}

StartMenu::~StartMenu() {
    // Clean up any resources if necessary
    wz_destroy(gui);
}

void StartMenu::loadAssets() {
    memset(&theme, 0, sizeof(theme));
    memcpy(&theme, &wz_def_theme, sizeof(theme));
    font = al_create_builtin_font();
    theme.font = font;
    theme.color1 = al_map_rgba_f(0.1, 0.1, 0.9, 1);
    theme.color2 = al_map_rgba_f(1, 0.6, 0.1, 1);

    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME*)&theme);
    
    wz_create_fill_layout(gui, 50 * size, 50 * size, 300 * size, 450 * size, 50 * size, 20 * size, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
    wz_create_textbox(gui, 0, 0, 200 * size, 50 * size, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("FLAPPY BIRD"), 1, -1);
    wz_create_button(gui, 100, 100, 150, 40, al_ustr_new("Clique aqui"), 1, 101);
    wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("Toggle 1"), 1, 1, 5);
	wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("Toggle 2"), 1, 1, 6);
	wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("Toggle 3"), 1, 1, 7);

    // Register the event sources for WidgetZ
    // wz_register_sources(gui, guiQueue);
}

void StartMenu::processEvent(const ALLEGRO_EVENT& event) {
    wz_send_event(gui, &event);

    switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            break;
        case WZ_BUTTON_PRESSED:
            std::cout << "WZ-BUTTON" << std::endl;
            if ((int)event.user.data1 == 101) {
                std::cout << "Botão da interface clicado!" << std::endl;
            }
            break;
    }
}

// void GameScene::processEvent(const ALLEGRO_EVENT& event) {
//     switch (event.type) {
//         case ALLEGRO_EVENT_KEY_DOWN:
//             if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
//                 bird.jump();
//             }
//             break;
//     }
// }

void StartMenu::update(float deltaTime) {
    wz_update(gui, deltaTime);
}

void StartMenu::gameOver() {
    //sceneManager->set_current_scene(std::make_unique<StartMenu>(sceneManager));
}

void StartMenu::draw() {    
    wz_draw(gui);
}