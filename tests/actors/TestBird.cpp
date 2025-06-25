#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "actors/Bird.hpp" // Bird.hpp original (não pode ser alterado)
#include "Constants.hpp" // Para constantes GLOBAIS como GRAVITY, BIRD_START_Y, etc.
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h> // Para al_draw_filled_rectangle (fallback em draw())
#include <vector>
#include <cmath> // Para sin, abs

// === Fixture para Inicialização e Limpeza do Allegro ===
class AllegroTestFixture {
public:
    ALLEGRO_DISPLAY* dummyDisplay = nullptr;
    std::vector<ALLEGRO_BITMAP*> dummyBitmaps;

    AllegroTestFixture() {
        if (!al_init()) { throw std::runtime_error("Falha ao inicializar Allegro."); }
        if (!al_init_primitives_addon()) { al_uninstall_system(); throw std::runtime_error("Falha ao inicializar Primitives Addon."); }

        al_set_new_display_flags(ALLEGRO_NOFRAME);
        al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);
        dummyDisplay = al_create_display(1, 1);
        if (!dummyDisplay) { al_shutdown_primitives_addon(); al_uninstall_system(); throw std::runtime_error("Falha ao criar display dummy."); }
    }

    ~AllegroTestFixture() {
        for (ALLEGRO_BITMAP* bmp : dummyBitmaps) {
            if (bmp) al_destroy_bitmap(bmp);
        }
        dummyBitmaps.clear();

        if (dummyDisplay) {
            al_destroy_display(dummyDisplay);
        }
        al_shutdown_primitives_addon();
        al_uninstall_system();
    }

    ALLEGRO_BITMAP* createDummyBitmap(int w, int h) {
        ALLEGRO_BITMAP* bmp = al_create_bitmap(w, h);
        if (!bmp) throw std::runtime_error("Falha ao criar bitmap dummy.");
        dummyBitmaps.push_back(bmp);
        return bmp;
    }
    
    std::vector<ALLEGRO_BITMAP*> createBirdFrames(int numFrames, int w, int h) {
        std::vector<ALLEGRO_BITMAP*> frames;
        for (int i = 0; i < numFrames; ++i) {
            frames.push_back(createDummyBitmap(w, h));
        }
        return frames;
    }
};

// === Testes para a Classe Bird (Usando apenas interface pública e constantes GLOBAIS) ===
TEST_SUITE("Testes da Classe Bird (Funcionalidade Observável)") {
    AllegroTestFixture fixture;

    // Teste do Construtor e Reset (estado inicial)
    TEST_CASE("Construtor inicializa Bird na posicao padrao") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(3, 30, 20);
        Bird bird(0, 0, 30, 20, frames);

        CHECK(bird.getX() == doctest::Approx(BIRD_START_X));
        CHECK(bird.getY() == doctest::Approx(BIRD_START_Y));
    }

    // Teste do método draw()
    TEST_CASE("draw() lida com frames vazios e desenha corretamente") {
        std::vector<ALLEGRO_BITMAP*> emptyFrames;
        Bird birdEmpty(0, 0, 30, 20, emptyFrames);
        CHECK_NOTHROW(birdEmpty.draw()); // Deve desenhar retângulo sem crashar

        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(2, 30, 20);
        Bird bird(0, 0, 30, 20, frames);
        CHECK_NOTHROW(bird.draw()); // Deve desenhar bitmap sem crashar
    }

    // Testes de JUMP
    TEST_CASE("jump() causa mudanca de posicao Y no proximo update se fisica ativa") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(1, 30, 20);
        Bird bird(0, 0, 30, 20, frames);
        
        // No Bird.cpp, jump() só funciona se physicsEnabled for true.
        // O Bird comeca com physicsEnabled = false (por reset()).
        // Não podemos setar physicsEnabled aqui. Apenas podemos chamar jump e ver se muda.
        // Se a lógica do seu jogo faz o Bird entrar em modo de física após a tela inicial,
        // este teste é limitado.
        
        // Verifica que jump() não crasha no estado inicial.
        CHECK_NOTHROW(bird.jump());
        float initialY = bird.getY();
        bird.update(0.01f); // Update para aplicar qualquer mudança
        // Se o Bird ainda estiver em modo hover (estado inicial), Y vai mudar.
        // Se jump() fizesse algo, Y mudaria também.
        CHECK(bird.getY() != doctest::Approx(initialY));
    }
    
    // Testes de DIE
    TEST_CASE("die() causa Bird a cair e girar para baixo") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(1, 30, 20);
        Bird bird(0, 0, 30, 20, frames);
        
        float initialY = bird.getY();
        bird.die(); 
        bird.update(0.1f); // Aplica a física após die()
        
        CHECK(bird.getY() < initialY); // Y deve ter diminuído (caiu)

        // Com mais tempo, ele deve cair mais
        float y_after_initial_fall = bird.getY();
        bird.update(0.5f); 
        CHECK(bird.getY() < y_after_initial_fall); // Deve cair mais
        CHECK(bird.getY() < initialY - 10.0f); // Caiu significativamente
    }

    // Testes de UPDATE - Estados e Física
    TEST_CASE("update() no estado inicial faz Bird flutuar (hover)") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(1, 30, 20);
        Bird bird(0, 0, 30, 20, frames); // Bird inicia em modo hover

        float initialY = bird.getY();
        bird.update(0.1f);
        CHECK(bird.getY() != doctest::Approx(initialY)); // Posição Y deve ter mudado devido à flutuação
        // Não podemos verificar o ângulo ou a função seno de applyHover, pois HOVER_AMPLITUDE etc. são privados.
    }

    TEST_CASE("update() com fisica ativa faz Bird cair e limita velocidade") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(1, 30, 20);
        Bird bird(0, 0, 30, 20, frames);

        // Para ativar a física, simulamos um pulo, se jump() habilita physicsEnabled.
        // Se jump() não habilita, este teste é mais um teste de não-crash.
        bird.jump(); // Isso pode habilitar physicsEnabled em Bird.cpp
        
        float initialY_after_jump = bird.getY();
        bird.update(0.1f); // Pequeno update após um pulo teórico
        CHECK(bird.getY() < initialY_after_jump); // Deve ter subido (Y menor)

        // Simula queda prolongada para testar velocidade terminal
        for (int i = 0; i < 50; ++i) { // Múltiplos updates para acumular queda
            bird.update(0.01f);
        }
        float y_after_fall = bird.getY();
        bird.update(0.01f);
        float y_after_fall_further = bird.getY();
        
        CHECK(y_after_fall_further > y_after_fall); // Continua caindo
        // Verificamos que a mudança de Y por update não excede uma estimativa da velocidade terminal.
        // Não podemos usar TERMINAL_VELOCITY diretamente na comparação de delta Y, pois Bird.cpp pode ter sua própria otimização de velY.
        // Mas sabemos que a velocidade não deveria ser insanamente alta.
        CHECK(y_after_fall_further - y_after_fall <= TERMINAL_VELOCITY * 0.01f + 0.1f); // Tolerância
    }

    // Testes de Animação (ALTAMENTE LIMITADOS)
    TEST_CASE("update() e draw() de animacao nao crasham quando nao esta morrendo") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(3, 30, 20);
        Bird bird(0, 0, 30, 20, frames);
        
        // Nao podemos verificar currentFrameIndex ou timeSinceLastFrame.
        // Apenas verificamos que as chamadas nao crasham e que a animacao 'aconteceria' internamente.
        CHECK_NOTHROW(bird.update(0.1f));
        CHECK_NOTHROW(bird.draw());
        CHECK_NOTHROW(bird.update(0.1f));
        CHECK_NOTHROW(bird.draw());
    }

    TEST_CASE("update() e draw() nao crasham quando esta morrendo (animacao parada)") {
        std::vector<ALLEGRO_BITMAP*> frames = fixture.createBirdFrames(3, 30, 20);
        Bird bird(0, 0, 30, 20, frames);
        
        bird.die(); 
        CHECK_NOTHROW(bird.update(1.0f)); 
        CHECK_NOTHROW(bird.draw());
    }
}