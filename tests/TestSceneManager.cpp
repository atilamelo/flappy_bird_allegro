#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/SceneManager.hpp"
#include "../include/Scene.hpp"
#include <memory>
#include <allegro5/allegro.h>
#include <cstring>

// Classe DummyScene usada para simular uma cena real durante os testes
class DummyScene : public Scene {
public:
    // Construtor necessário, pois Scene requer um ponteiro para SceneManager
    DummyScene() : Scene(nullptr) {}

    // Flags para verificar se os métodos foram chamados
    bool loadCalled     = false;
    bool eventProcessed = false;
    bool updated        = false;
    bool drawn          = false;

    // Sobrescreve os métodos virtuais para marcar chamadas
    void loadAssets() override            { loadCalled = true; }
    void processEvent(const ALLEGRO_EVENT&) override { eventProcessed = true; }
    void update(float) override           { updated = true; }
    void draw() override                  { drawn = true; }
};

TEST_SUITE("SceneManager Tests") {

    // Testa o estado inicial do gerenciador e o método shutdown()
    TEST_CASE("Estado inicial é running e shutdown interrompe") {
        SceneManager mgr;
        CHECK(mgr.is_running() == true); // Deve iniciar em execução
        mgr.shutdown();
        CHECK(mgr.is_running() == false); // Deve parar após shutdown
    }

    // Verifica se a troca de cena só ocorre após update()
    TEST_CASE("set_current_scene adia o load até update()") {
        SceneManager mgr;
        auto scene = std::make_unique<DummyScene>();
        DummyScene* ptr = scene.get();
        mgr.set_current_scene(std::move(scene));

        // Antes do update, loadAssets NÃO deve ter sido chamado
        CHECK(ptr->loadCalled == false);

        // processEvent antes do update não deve fazer nada
        ALLEGRO_EVENT ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.type = ALLEGRO_EVENT_KEY_DOWN;
        mgr.processEvent(ev);
        CHECK(ptr->eventProcessed == false);

        // update deve realizar a transição e chamar loadAssets e update
        mgr.update(0.1f);
        CHECK(ptr->loadCalled == true);
        CHECK(ptr->updated == true);

        // draw() deve chamar o método draw da cena atual
        mgr.draw();
        CHECK(ptr->drawn == true);
    }

    // Garante que chamadas a métodos sem cena atual não causam falhas
    TEST_CASE("Chamadas em vazio não causam crash") {
        SceneManager mgr;
        ALLEGRO_EVENT ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.type = ALLEGRO_EVENT_KEY_DOWN;

        // As chamadas abaixo não devem lançar exceções mesmo sem cena
        CHECK_NOTHROW(mgr.processEvent(ev));
        CHECK_NOTHROW(mgr.draw());
    }

    // Testa se set_current_scene sobrescreve a cena anterior antes de carregar
    TEST_CASE("Novas set_current_scene sobrescrevem anteriores") {
        SceneManager mgr;
        auto first  = std::make_unique<DummyScene>();
        auto second = std::make_unique<DummyScene>();
        DummyScene* ptr2 = second.get();

        mgr.set_current_scene(std::move(first));  // Ignorado
        mgr.set_current_scene(std::move(second)); // Este será o carregado

        // Somente a segunda cena deve ser carregada após o update
        mgr.update(0.0f);
        CHECK(ptr2->loadCalled == true);
    }

}
