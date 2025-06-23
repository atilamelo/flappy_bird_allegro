#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <actors/Floor.hpp>
#include "Constants.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

// Inicializa Allegro e o addon no início de cada teste
static void init_allegro() {
    static bool initialized = false;
    if (!initialized) {
        if (!al_init()) {
            throw std::runtime_error("Falha ao inicializar Allegro");
        }
        if (!al_init_primitives_addon()) {
            throw std::runtime_error("Falha ao inicializar Allegro Primitives Addon");
        }
        initialized = true;
    }
}

TEST_CASE("Construtor de Floor lança exceção se textura for nula") {
    init_allegro();
    CHECK_THROWS_AS(Floor(nullptr), std::runtime_error);
}

TEST_CASE("Floor::update desloca e reinicia posição quando sai da tela") {
    init_allegro();

    // Cria uma textura dummy
    ALLEGRO_BITMAP* texturaDummy = al_create_bitmap(10, 10);
    REQUIRE(texturaDummy != nullptr);

    Floor floor(texturaDummy);

    // Define posição além do limite esquerdo
    floor.setX(-BUFFER_W - 1);

    // Chama update com dt = 1.0
    floor.update(1.0f);

    // Verifica se a posição foi reiniciada para 0
    CHECK(floor.getX() == doctest::Approx(0.0f));

    al_destroy_bitmap(texturaDummy);
}
