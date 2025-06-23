#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/managers/SceneManager.hpp"
#include "../include/core/Scene.hpp"
#include <memory>
#include <allegro5/allegro.h>

class DummyScene : public Scene {
public:
    DummyScene() : Scene(nullptr) {}

    bool loadCalled     = false;
    bool eventProcessed = false;
    bool updated        = false;
    bool drawn          = false;

    void loadAssets() override            { loadCalled = true; }
    void processEvent(const ALLEGRO_EVENT&) override { eventProcessed = true; }
    void update(float) override           { updated = true; }
    void draw() override                  { drawn = true; }
};

TEST_SUITE("SceneManager Tests") {
    TEST_CASE("Initial state and shutdown") {
        SceneManager mgr;
        CHECK(mgr.is_running());
        mgr.shutdown();
        CHECK_FALSE(mgr.is_running());
    }

    TEST_CASE("set_current_scene delays activation until update") {
        SceneManager mgr;
        auto scene = std::make_unique<DummyScene>();
        DummyScene* ptr = scene.get();
        
        mgr.set_current_scene(std::move(scene));
        
        // Cena não deve estar ativa antes do update
        ALLEGRO_EVENT ev{};
        ev.type = ALLEGRO_EVENT_KEY_DOWN;
        mgr.processEvent(ev);
        CHECK_FALSE(ptr->eventProcessed);

        // Update ativa a cena mas NÃO chama loadAssets()
        mgr.update(0.1f);
        CHECK(ptr->updated);      // update foi chamado
        CHECK_FALSE(ptr->loadCalled); // Implementação original não chama loadAssets()

        // Após update, a cena deve responder a eventos
        mgr.processEvent(ev);
        CHECK(ptr->eventProcessed);

        mgr.draw();
        CHECK(ptr->drawn);
    }

    TEST_CASE("Safe calls with no scene") {
        SceneManager mgr;
        ALLEGRO_EVENT ev{};
        ev.type = ALLEGRO_EVENT_KEY_DOWN;

        SUBCASE("Process event") { CHECK_NOTHROW(mgr.processEvent(ev)); }
        SUBCASE("Draw")          { CHECK_NOTHROW(mgr.draw()); }
        SUBCASE("Update")        { CHECK_NOTHROW(mgr.update(0.1f)); }
    }

    TEST_CASE("Multiple set_current_scene overwrites previous") {
        SceneManager mgr;
        auto first  = std::make_unique<DummyScene>();
        auto second = std::make_unique<DummyScene>();
        DummyScene* ptr1 = first.get();
        DummyScene* ptr2 = second.get();

        mgr.set_current_scene(std::move(first));
        mgr.set_current_scene(std::move(second));
        
        mgr.update(0.0f);
        
        CHECK(ptr2->updated);    // Segunda cena foi atualizada
        CHECK_FALSE(ptr1->updated); // Primeira nunca foi ativada
    }

    TEST_CASE("Scene doesn't load assets automatically") {
        SceneManager mgr;
        auto scene = std::make_unique<DummyScene>();
        DummyScene* ptr = scene.get();
        
        mgr.set_current_scene(std::move(scene));
        mgr.update(0.0f);
        
        // Verifica que loadAssets() NÃO foi chamado
        CHECK_FALSE(ptr->loadCalled);
    }
}