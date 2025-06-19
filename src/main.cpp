#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <Constants.hpp>
#include <chrono>
#include "SceneManager.hpp"
#include "GameScene.hpp"
#include "util/ResourceManager.hpp"

void must_init(bool test, const char* description) {
    if (test) return;
    printf("Não pode inicializar %s\n", description);
    exit(1);
}

int main() {
    std::chrono::steady_clock::time_point lastUpdate;

    must_init(al_init(), "Allegro");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font addon");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "image addon");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* display = al_create_display(BUFFER_W, BUFFER_H);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    SceneManager scene_manager = SceneManager();
    ResourceManager::getInstance().loadAtlasJson("assets/sprites/sprite_sheet.json", "atlas", "assets/sprites/sprite_sheet.png");

    al_start_timer(timer);
    bool redraw = true;

    lastUpdate = std::chrono::steady_clock::now();

    scene_manager.set_current_scene(std::make_unique<GameScene>(&scene_manager));

    while (scene_manager.is_running()) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            scene_manager.update(1.0 / FPS);
            redraw = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            scene_manager.shutdown();
        }

        scene_manager.processEvent(event);

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            scene_manager.draw();
            al_flip_display();
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}