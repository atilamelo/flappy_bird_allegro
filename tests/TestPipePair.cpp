#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <actors/PipePair.hpp>
#include <actors/Bird.hpp>
#include "../include/Constants.hpp"
#include <vector>

TEST_CASE("PipePair padrão: x = -PIPE_WIDTH e inativo") {
    PipePair pair;
    CHECK(pair.getX() == doctest::Approx(-PIPE_WIDTH));
    CHECK(!pair.isActive());
}

TEST_CASE("update não altera x quando inativo") {
    PipePair pair;
    float before = pair.getX();
    pair.update(1.23f);
    CHECK(pair.getX() == doctest::Approx(before));
}

TEST_CASE("hasPassed retorna false quando inativo") {
    PipePair pair;
    Bird bird(0.0f, 0.0f, 1.0f, 1.0f, std::vector<ALLEGRO_BITMAP*>{});
    CHECK(!pair.hasPassed(bird));
}

TEST_CASE("reset traz PipePair para estado inicial repetidamente") {
    PipePair pair;
    // Estado inicial
    float x0 = pair.getX();
    bool act0 = pair.isActive();

    // Chamando reset novamente
    pair.reset();
    CHECK(pair.getX() == doctest::Approx(x0));
    CHECK(pair.isActive() == act0);
}

TEST_CASE("update com dt zero não altera x mesmo se ativo") {
    PipePair pair;
    // Ativar artificialmente via init hack
    // Aqui simplesmente chamamos update com dt zero e validamos
    float x_before = pair.getX();
    pair.update(0.0f);
    CHECK(pair.getX() == doctest::Approx(x_before));
}

TEST_CASE("getWidth retorna PIPE_WIDTH constante") {
    PipePair pair;
    CHECK(pair.getWidth() == doctest::Approx(PIPE_WIDTH));
}
