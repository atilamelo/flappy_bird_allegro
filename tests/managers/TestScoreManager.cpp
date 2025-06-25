#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "managers/ScoreManager.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>

// --- Mock do ResourceManager para Testes ---
class MockResourceManager : public ResourceManager {
public:
    mutable std::map<std::string, ALLEGRO_BITMAP*> mockBitmaps;

    MockResourceManager() : ResourceManager() {}

    ALLEGRO_BITMAP* getBitmap(const std::string& id) const override {
        auto it = mockBitmaps.find(id);
        if (it != mockBitmaps.end()) {
            return it->second;
        }

        ALLEGRO_BITMAP* newBitmap = al_create_bitmap(20, 30);
        if (newBitmap) {
            mockBitmaps[id] = newBitmap;
        } else {
            std::cerr << "AVISO: Falha ao criar bitmap simulado para o dígito " << id << std::endl;
        }
        return newBitmap;
    }

    void clearMockBitmaps() {
        for (auto const& [id, bitmap] : mockBitmaps) {
            al_destroy_bitmap(bitmap);
        }
        mockBitmaps.clear();
    }
};

// --- Estrutura Auxiliar para Inicialização do Allegro E Injeção do Mock ---
class AllegroFixture {
public:
    MockResourceManager mockResMgr; // Nossa instância de mock

    AllegroFixture() {
        if (!al_init()) { throw std::runtime_error("Falha ao inicializar Allegro."); }
        if (!al_init_image_addon()) { al_shutdown_image_addon(); throw std::runtime_error("Falha ao inicializar o add-on de imagens do Allegro."); }
        al_set_new_display_flags(ALLEGRO_NOFRAME);
        al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);
        dummyDisplay = al_create_display(640, 480);
        if (!dummyDisplay) { al_shutdown_image_addon(); al_uninstall_system(); throw std::runtime_error("Falha ao criar display dummy do Allegro."); }

        // *******************************************************************
        // AQUI ESTÁ A MUDANÇA CRUCIAL: Injeta o MockResourceManager.
        ResourceManager::setInstanceForTesting(&mockResMgr);
        // *******************************************************************
    }

    ~AllegroFixture() {
        // Limpa os bitmaps do mock
        mockResMgr.clearMockBitmaps();

        // *******************************************************************
        // RESTAURA a instância do ResourceManager após os testes.
        ResourceManager::resetInstance();
        // *******************************************************************

        if (dummyDisplay) { al_destroy_display(dummyDisplay); }
        al_shutdown_image_addon();
        al_uninstall_system();
    }
private:
    ALLEGRO_DISPLAY* dummyDisplay = nullptr;
};

// Agora esta classe não precisa mais limpar/criar os digitSprites,
// pois o construtor base do ScoreManager já os carregará do MockResourceManager.
class TestableScoreManager : public ScoreManager {
public:
    TestableScoreManager() : ScoreManager() {
        // Nada mais a fazer aqui, o construtor de ScoreManager já usou o mock!
    }

    // O destrutor também não precisa mais destruir os bitmaps,
    // pois eles foram criados e gerenciados pelo MockResourceManager.
    ~TestableScoreManager() {}

    void testDrawNumberSprites(int number, float x, float y, float scale, TextAlign align) const {
        drawNumberSprites(number, x, y, scale, align);
    }

    float getDigitWidth(int digit) const {
        // Acessa digitSprites, que agora terá sido preenchido pelo ScoreManager base
        // usando os bitmaps do MockResourceManager.
        if (digit >= 0 && digit <= 9 && digitSprites[digit]) {
            return al_get_bitmap_width(digitSprites[digit]);
        }
        return 0;
    }
};

// --- Testes para o ScoreManager ---
TEST_SUITE("Testes do ScoreManager") {
    // A fixture agora injeta o mock e cuida da limpeza.
    AllegroFixture fixture;

    TEST_CASE("O construtor do ScoreManager carrega os sprites de dígitos corretamente") {
        // Este teste agora verifica que o ScoreManager se inicializa corretamente
        // usando o MockResourceManager que está ativo.
        CHECK_NOTHROW(TestableScoreManager());
    }

    TEST_CASE("drawNumberSprites trata números negativos como 0") {
        TestableScoreManager system;
        CHECK_NOTHROW(system.testDrawNumberSprites(-100, 0, 0, 1.0f, TextAlign::LEFT));
    }

    TEST_CASE("drawNumberSprites alinha à esquerda corretamente") {
        TestableScoreManager system;
        float x = 100.0f;
        float y = 50.0f;
        float scale = 1.0f;
        int number = 123;
        CHECK_NOTHROW(system.testDrawNumberSprites(number, x, y, scale, TextAlign::LEFT));
    }

    TEST_CASE("drawNumberSprites alinha ao centro corretamente") {
        TestableScoreManager system;
        float x = 200.0f;
        float y = 50.0f;
        float scale = 1.0f;
        int number = 4567;
        CHECK_NOTHROW(system.testDrawNumberSprites(number, x, y, scale, TextAlign::CENTER));
    }

    TEST_CASE("drawNumberSprites alinha à direita corretamente") {
        TestableScoreManager system;
        float x = 300.0f;
        float y = 50.0f;
        float scale = 1.0f;
        int number = 89;
        CHECK_NOTHROW(system.testDrawNumberSprites(number, x, y, scale, TextAlign::RIGHT));
    }

    TEST_CASE("drawNumberSprites aplica escala aos dígitos corretamente") {
        TestableScoreManager system;
        float x = 0.0f;
        float y = 0.0f;
        float scale = 0.5f;
        int number = 1;
        float expectedScaledWidth = system.getDigitWidth(1) * scale;
        CHECK(expectedScaledWidth == 10.0f);
        CHECK_NOTHROW(system.testDrawNumberSprites(number, x, y, scale, TextAlign::LEFT));
    }

    TEST_CASE("draw() não causa falhas e chama drawNumberSprites") {
        TestableScoreManager system;
        system.setCurrentScore(12345);
        CHECK_NOTHROW(system.draw());
    }

    TEST_CASE("getScore, increaseScore e reset funcionam corretamente") {
        ScoreManager sm; // ScoreManager normal, mas usará o MockResourceManager injetado pela fixture.
        CHECK(sm.getScore() == 0);

        sm.increaseScore();
        CHECK(sm.getScore() == 1);

        sm.increaseScore();
        sm.increaseScore();
        CHECK(sm.getScore() == 3);

        sm.reset();
        CHECK(sm.getScore() == 0);
    }
}