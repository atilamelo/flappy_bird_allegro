#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <actors/Pipe.hpp>
#include "Constants.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

// Inicializa Allegro e addons uma única vez
static void init_allegro_pipe() {
    static bool initialized = false;
    if (!initialized) {
        if (!al_init()) {
            throw std::runtime_error("Falha ao inicializar Allegro");
        }
        if (!al_init_primitives_addon()) {
            throw std::runtime_error("Falha ao inicializar Primitives Addon");
        }
        initialized = true;
    }
}

TEST_CASE("Pipe::draw sem textura usa desenho de retângulo verde") {
    init_allegro_pipe();
    // Configura um bitmap de destino para drawing
    ALLEGRO_DISPLAY* disp = al_create_display(100, 100);
    REQUIRE(disp != nullptr);

    Pipe pipe(20.0f, 10.0f, 30.0f, PipeType::BOTTOM, nullptr);
    // Deve desenhar retângulo sem lançar exceção
    CHECK_NOTHROW(pipe.draw(5.0f));

    al_destroy_display(disp);
}

TEST_CASE("Pipe::draw com textura tipo TOP usa desenho rotacionado") {
    init_allegro_pipe();
    ALLEGRO_DISPLAY* disp = al_create_display(100, 100);
    REQUIRE(disp != nullptr);
    
    ALLEGRO_BITMAP* bmp = al_create_bitmap(10, 20);
    REQUIRE(bmp != nullptr);

    Pipe pipeTop(10.0f, 10.0f, 20.0f, PipeType::TOP, bmp);
    CHECK_NOTHROW(pipeTop.draw(15.0f));

    al_destroy_bitmap(bmp);
    al_destroy_display(disp);
}

TEST_CASE("Pipe::draw com textura tipo BOTTOM usa al_draw_bitmap") {
    init_allegro_pipe();
    ALLEGRO_DISPLAY* disp = al_create_display(100, 100);
    REQUIRE(disp != nullptr);
    
    ALLEGRO_BITMAP* bmp = al_create_bitmap(8, 12);
    REQUIRE(bmp != nullptr);

    Pipe pipeBot(50.0f, 8.0f, 12.0f, PipeType::BOTTOM, bmp);
    CHECK_NOTHROW(pipeBot.draw(0.0f));

    al_destroy_bitmap(bmp);
    al_destroy_display(disp);
}
