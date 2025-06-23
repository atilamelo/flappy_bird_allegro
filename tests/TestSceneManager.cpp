#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/managers/SceneManager.hpp"
#include "../include/core/Scene.hpp"
#include <memory>
#include <allegro5/allegro.h>

// Classe DummyScene usada para simular uma cena real durante os testes
class DummyScene : public Scene {
public:
    DummyScene() : Scene(nullptr) {}

    // Flags para verificar se os métodos foram chamados
    bool eventProcessed = false;
    bool updated        = false;
    bool drawn          = false;

    // Sobrescreve os métodos virtuais para marcar chamadas
    void processEvent(const ALLEGRO_EVENT&) override { eventProcessed = true; }
    void update(float) override           { updated = true; }
    void draw() override                  { drawn = true; }
};

TEST_SUITE("Testes do SceneManager") {

    // Testa o estado inicial do gerenciador e o método shutdown()
    TEST_CASE("Estado inicial é running e shutdown interrompe") {
        SceneManager mgr;
        CHECK(mgr.is_running() == true); // O gerenciador deve iniciar ativo
        mgr.shutdown();
        CHECK(mgr.is_running() == false); // Após shutdown, deve encerrar corretamente
    }

    // Verifica se a troca de cena é adiada até a chamada de update()
    TEST_CASE("setCurrentScene adia a ativação até update()") {
        SceneManager mgr;
        auto scene = std::make_unique<DummyScene>();
        DummyScene* ptr = scene.get(); // Ponteiro para verificar flags
        
        mgr.setCurrentScene(std::move(scene));

        // Cena não deve estar ativa antes do update
        ALLEGRO_EVENT ev;
        ev.type = ALLEGRO_EVENT_KEY_DOWN;
        mgr.processEvent(ev);
        CHECK(ptr->eventProcessed == false); // Evento não processado

        // Update deve ativar a nova cena
        mgr.update(0.1f);
        CHECK(ptr->updated == true); // Update chamado após ativação

        // Após update, deve processar eventos
        mgr.processEvent(ev);
        CHECK(ptr->eventProcessed == true); // Evento processado

        // Draw deve delegar para o método da cena
        mgr.draw();
        CHECK(ptr->drawn == true); // Desenho realizado
    }

    // Garante robustez: sem cena ativa, as chamadas não devem causar erro
    TEST_CASE("Chamadas em vazio não causam crash") {
        SceneManager mgr;
        ALLEGRO_EVENT ev;
        ev.type = ALLEGRO_EVENT_KEY_DOWN;

        // Nenhuma das chamadas deve lançar exceção mesmo sem cena definida
        SUBCASE("Processar evento") { CHECK_NOTHROW(mgr.processEvent(ev)); }
        SUBCASE("Desenhar")         { CHECK_NOTHROW(mgr.draw()); }
        SUBCASE("Atualizar")        { CHECK_NOTHROW(mgr.update(0.1f)); }
    }

    // Testa se múltiplas chamadas a setCurrentScene sobrescrevem a anterior
    TEST_CASE("Novas setCurrentScene sobrescrevem anteriores") {
        SceneManager mgr;
        auto first  = std::make_unique<DummyScene>();
        auto second = std::make_unique<DummyScene>();
        DummyScene* ptr1 = first.get();
        DummyScene* ptr2 = second.get();

        mgr.setCurrentScene(std::move(first));  // Substituída antes do uso
        mgr.setCurrentScene(std::move(second)); // Esta deve ser ativada
        
        // Apenas a última cena definida deve ser ativada
        mgr.update(0.0f);
        CHECK(ptr2->updated == true);    // Segunda cena atualizada
        CHECK(ptr1->updated == false);   // Primeira cena nunca ativada
    }
}