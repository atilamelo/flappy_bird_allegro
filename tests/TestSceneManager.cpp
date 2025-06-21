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
        CHECK(mgr.is_running() == true); // O gerenciador deve iniciar ativo
        mgr.shutdown();
        CHECK(mgr.is_running() == false); // Após shutdown, deve encerrar corretamente
    }

    // Verifica se a troca de cena é adiada até a chamada de update()
    TEST_CASE("set_current_scene adia o load até update()") {
        SceneManager mgr;
        auto scene = std::make_unique<DummyScene>();
        DummyScene* ptr = scene.get(); // Ponteiro para verificar flags
        mgr.set_current_scene(std::move(scene));

        // loadAssets ainda não deve ter sido chamado antes de update()
        CHECK(ptr->loadCalled == false);

        // Eventos antes do update devem ser ignorados
        ALLEGRO_EVENT ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.type = ALLEGRO_EVENT_KEY_DOWN;
        mgr.processEvent(ev);
        CHECK(ptr->eventProcessed == false);

        // update deve aplicar a nova cena e chamar loadAssets e update
        mgr.update(0.1f);
        CHECK(ptr->loadCalled == true);  // Cena carregada
        CHECK(ptr->updated == true);     // Atualizada após transição

        // draw() deve delegar para o método da cena
        mgr.draw();
        CHECK(ptr->drawn == true); // Verifica se foi desenhada
    }

    // Garante robustez: sem cena ativa, as chamadas não devem causar erro
    TEST_CASE("Chamadas em vazio não causam crash") {
        SceneManager mgr;
        ALLEGRO_EVENT ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.type = ALLEGRO_EVENT_KEY_DOWN;

        // Nenhuma das chamadas deve lançar exceção mesmo sem cena definida
        CHECK_NOTHROW(mgr.processEvent(ev));
        CHECK_NOTHROW(mgr.draw());
    }

    // Testa se múltiplas chamadas a set_current_scene sobrescrevem a anterior
    TEST_CASE("Novas set_current_scene sobrescrevem anteriores") {
        SceneManager mgr;
        auto first  = std::make_unique<DummyScene>();
        auto second = std::make_unique<DummyScene>();
        DummyScene* ptr2 = second.get(); // Cena que será efetivamente usada

        mgr.set_current_scene(std::move(first));  // Substituída antes do uso
        mgr.set_current_scene(std::move(second)); // Esta deve ser carregada

        // Apenas a última cena definida deve ser carregada e atualizada
        mgr.update(0.0f);
        CHECK(ptr2->loadCalled == true); // Verifica se a cena correta foi usada
    }

}

