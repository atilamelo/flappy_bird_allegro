#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <allegro5/allegro_image.h>
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
    al_init_image_addon();

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
    
    wz_create_fill_layout(gui, 0 * size, 0 * size, BUFFER_W, BUFFER_H, 0.8 * BUFFER_W, 0.0001 * BUFFER_H, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 2);
        wgt = (WZ_WIDGET*) wz_create_image_button(gui,0,0,288,512,back,back,back,back,9);
        wgt->flags = WZ_STATE_NOTWANT_FOCUS;
    wz_create_fill_layout(gui, 0 * size, 10, BUFFER_W, BUFFER_H, 0.8 * BUFFER_W, 0.1 * BUFFER_H, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 0);
        wgt = (WZ_WIDGET*) wz_create_image_button(gui,0,0,192,100,al_load_bitmap("data/gameover.png"),al_load_bitmap("data/gameover.png"),al_load_bitmap("data/gameover.png"),
        al_load_bitmap("data/gameover.png"),-2);
        wgt->flags = WZ_STATE_NOTWANT_FOCUS;
        wz_create_image_button(gui,0,0,40,14,al_load_bitmap("data/ok_button.png"),al_load_bitmap("data/ok_button_pressed.png"),al_load_bitmap("data/ok_button_focused.png"),
            al_load_bitmap("data/ok_button.png"),11);
        wz_create_image_button(gui,0,0,40,14,al_load_bitmap("data/menu_button.png"),al_load_bitmap("data/menu_button_pressed.png"),al_load_bitmap("data/menu_button_focused.png"),
            al_load_bitmap("data/menu_button.png"),12);
        wz_create_image_button(gui,0,0,40,14,al_load_bitmap("data/score_button.png"),al_load_bitmap("data/score_button_pressed.png"),al_load_bitmap("data/score_button_focused.png"),
            al_load_bitmap("data/score_button.png"),13);
        wz_create_image_button(gui,0,0,40,14,al_load_bitmap("data/quit_button.png"),al_load_bitmap("data/quit_button_pressed.png"),al_load_bitmap("data/quit_button_focused.png"),
            al_load_bitmap("data/quit_button.png"),14);

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