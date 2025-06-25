#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN  
#include "../doctest/doctest.h"
#include <actors/ParallaxBackground.hpp>
#include "../include/Constants.hpp"  
#include <allegro5/allegro5.h>
#include <vector>

// Inicialização única de Allegro para testes de ParallaxBackground
static void init_allegro_pb() {
    static bool initialized = false;
    if (!initialized) {
        if (!al_init()) throw std::runtime_error("Falha ao inicializar Allegro");
        // Cria display mínimo (1x1)
        ALLEGRO_DISPLAY* disp = al_create_display(1, 1);
        if (!disp) throw std::runtime_error("Falha ao criar display para ParallaxBackground");
        initialized = true;
    }
}

TEST_CASE("Construtor: textura nula mantém width e height em zero") {
    init_allegro_pb();
    ParallaxBackground bg(nullptr, 5.0f);
    CHECK(bg.getWidth() == doctest::Approx(0.0f));
    CHECK(bg.getHeight() == doctest::Approx(0.0f));
    CHECK(bg.getX() == doctest::Approx(0.0f));
}

TEST_CASE("Construtor: com textura valida ajusta dimensoes corretamente") {
    init_allegro_pb();
    // Cria bitmap dummy de dimensoes 5x10
    ALLEGRO_BITMAP* bmp = al_create_bitmap(5, 10);
    REQUIRE(bmp != nullptr);

    ParallaxBackground bg(bmp, 3.0f);
    CHECK(bg.getWidth() == doctest::Approx(5.0f));
    CHECK(bg.getHeight() == doctest::Approx(10.0f));
    CHECK(bg.getX() == doctest::Approx(0.0f));

    al_destroy_bitmap(bmp);
}

TEST_CASE("Update diminui x de acordo com speed e dt e realiza wrap") {
    init_allegro_pb();
    ALLEGRO_BITMAP* bmp = al_create_bitmap(4, 2);
    REQUIRE(bmp != nullptr);

    float speed = 2.0f;
    ParallaxBackground bg(bmp, speed);

    // Atualiza com dt=1.0 => x = -2
    bg.update(1.0f);
    CHECK(bg.getX() == doctest::Approx(-2.0f));

    // Atualiza com dt tal que x <= -width: dt = (width+1)/speed = (4+1)/2 = 2.5 => x = -2 -5 = -7 => wrap: x = -7+4 = -3
    bg.update(2.5f);
    CHECK(bg.getX() == doctest::Approx(-3.0f));

    al_destroy_bitmap(bmp);
}

TEST_CASE("Update com dt zero nao altera x") {
    init_allegro_pb();
    ALLEGRO_BITMAP* bmp = al_create_bitmap(3,3);
    REQUIRE(bmp != nullptr);

    ParallaxBackground bg(bmp, 5.0f);
    float x0 = bg.getX();
    bg.update(0.0f);
    CHECK(bg.getX() == doctest::Approx(x0));

    al_destroy_bitmap(bmp);
}
