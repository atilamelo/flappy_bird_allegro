#include "StartMenu.hpp"
#include "Scene.hpp"
#include "Constants.hpp"
#include "SceneManager.hpp"
#include "GameScene.hpp"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include "widgetz/widgetz.h"

StartMenu::StartMenu(SceneManager *sceneManager) 
    : Scene(sceneManager) {
    loadAssets();
}

StartMenu::~StartMenu() {
    // Clean up any resources if necessary
    std::cout << "Destruindo StartMenu" << std::endl;
    wz_destroy(gui);
}

void StartMenu::loadAssets() {
    al_init_image_addon();

	memset(&skin_theme, 0, sizeof(skin_theme));
	memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
    font = al_create_builtin_font();
    skin_theme.theme.font = font;
    skin_theme.theme.color1 = al_map_rgba_f(0.37, 0.89, 0.44, 1);
    skin_theme.theme.color2 = al_map_rgba_f(0.99, 0.63, 0.27, 1);
	skin_theme.editbox_bitmap = al_load_bitmap("data/editbox.png");
	ALLEGRO_BITMAP* back= al_load_bitmap("data/background-day.png");
	// skin_theme.box_bitmap = back;

    wz_init_skin_theme(&skin_theme);
    gui = wz_create_widget(0, 0, 0, -1);
    wz_set_theme(gui, (WZ_THEME*)&skin_theme);

    wz_create_fill_layout(gui, 0 * size, 0 * size, BUFFER_W, BUFFER_H, 0.8 * BUFFER_W, 0.0001 * BUFFER_H, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 2);
        wgt = (WZ_WIDGET*) wz_create_image_button(gui,0,0,288,512,back,back,back,back,9);
        wgt->flags = WZ_STATE_NOTWANT_FOCUS;
    wz_create_fill_layout(gui, 0 * size, 10, BUFFER_W, BUFFER_H, 0.8 * BUFFER_W, 0.1 * BUFFER_H, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, 0);
        wgt = (WZ_WIDGET*) wz_create_image_button(gui,0,0,89,24,al_load_bitmap("data/flappy_bird.png"),al_load_bitmap("data/flappy_bird.png"),al_load_bitmap("data/flappy_bird.png"),
            al_load_bitmap("data/flappy_bird.png"),-2);
        wgt->flags = WZ_STATE_NOTWANT_FOCUS;
        editbox = wz_create_editbox(gui, 0, 0, 100 * size, 30 * size, al_ustr_new(""), 1, 10);
        wz_create_image_button(gui,0,0,52,29,al_load_bitmap("data/start_button.png"),al_load_bitmap("data/start_button_pressed.png"),al_load_bitmap("data/start_button_focused.png"),
            al_load_bitmap("data/start_button.png"),11);
        wz_create_image_button(gui,0,0,52,29,al_load_bitmap("data/rank_button.png"),al_load_bitmap("data/rank_button_pressed.png"),al_load_bitmap("data/rank_button_focused.png"),
            al_load_bitmap("data/rank_button.png"),12);
        wz_create_toggle_button(gui, 0, 0, 200 * size, 50 * size, al_ustr_new("QUIT"), 1, 1, 13);

    // Register the event sources for WidgetZ
    ALLEGRO_EVENT_QUEUE* queue = sceneManager->get_event_queue();
    wz_register_sources(gui, queue);
}

void StartMenu::processEvent(const ALLEGRO_EVENT& event) {
    //make a copy of the event to permit modification
    //this is necessary because WidgetZ modifies the event
    //to indicate that it has been handled
    ALLEGRO_EVENT e = event;

    bool handled = wz_send_event(gui, &e);

    switch (e.type) {
        case WZ_BUTTON_PRESSED:
            if ((int)e.user.data1 == 11) {
                std::cout << "Iniciar jogo!" << std::endl;
                if(editbox->text->slen > 3 && editbox->text->slen < 20)
                sceneManager->set_current_scene(std::make_unique<GameScene>(sceneManager));
            }
            if ((int)e.user.data1 == 12) {
                std::cout << "Placar!" << std::endl;
            }
            if ((int)e.user.data1 == 13) {
                std::cout << "Sair!" << std::endl;
                sceneManager->shutdown();
            }
            break;
    }
}

void StartMenu::update(float deltaTime) {
    wz_update(gui, deltaTime);
}

void StartMenu::gameOver() {

}

void StartMenu::draw() {    
    wz_draw(gui);
    ALLEGRO_BITMAP* foto = al_load_bitmap("data/flappy-bird.png");

    al_draw_bitmap(foto, 0, 0, 0);
}