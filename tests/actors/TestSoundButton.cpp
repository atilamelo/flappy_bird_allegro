// tests/actors/TestSoundButton.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "actors/SoundButton.hpp"

/**
 * @class MockGameSound
 * @brief Classe de simulação (mock) para GameSound.
 *
 * Esta classe simula o comportamento do GameSound para os testes. Em vez de
 * controlar o áudio, ela apenas registra se os métodos de mutar/desmutar
 * foram chamados, permitindo verificar a lógica do SoundButton sem precisar
 * de uma saída de áudio real.
 */
class MockGameSound {
public:
    bool mute_was_called = false;
    bool unmute_was_called = false;

    // Simula a ação de mutar a música
    void mute_music() {
        mute_was_called = true;
    }

    // Simula a ação de desmutar a música
    void unmute_music() {
        unmute_was_called = true;
    }

    // Reseta o estado do mock para um novo teste
    void reset() {
        mute_was_called = false;
        unmute_was_called = false;
    }
};

TEST_SUITE("SoundButton") {
    // Ponteiros dummy para simular os bitmaps. Não precisamos de imagens reais.
    ALLEGRO_BITMAP* dummy_img_on = (ALLEGRO_BITMAP*)1;
    ALLEGRO_BITMAP* dummy_img_off = (ALLEGRO_BITMAP*)2;

    TEST_CASE("Button initialization and direct state change") {
        MockGameSound mock_sound_manager;
        SoundButton button(10, 20, 50, 50, dummy_img_on, dummy_img_off, reinterpret_cast<GameSound*>(&mock_sound_manager));

        // Por padrão, o botão não deve estar mutado e não deve chamar nenhuma função
        CHECK_FALSE(mock_sound_manager.mute_was_called);
        CHECK_FALSE(mock_sound_manager.unmute_was_called);

        SUBCASE("Directly setting mute to true") {
            button.setMuted(true);
            CHECK(mock_sound_manager.mute_was_called);
            CHECK_FALSE(mock_sound_manager.unmute_was_called);
        }

        SUBCASE("Directly setting mute to false") {
            // Primeiro, mutamos para garantir que o estado mude
            button.setMuted(true);
            mock_sound_manager.reset(); // Limpa o estado do mock

            button.setMuted(false);
            CHECK_FALSE(mock_sound_manager.mute_was_called);
            CHECK(mock_sound_manager.unmute_was_called);
        }
    }

    TEST_CASE("Processing mouse click events") {
        MockGameSound mock_sound_manager;
        // Botão posicionado em (x=100, y=100) com tamanho 50x50
        SoundButton button(100, 100, 50, 50, dummy_img_on, dummy_img_off, reinterpret_cast<GameSound*>(&mock_sound_manager));

        ALLEGRO_EVENT event;
        event.type = ALLEGRO_EVENT_MOUSE_BUTTON_DOWN;

        SUBCASE("Clicking inside the button toggles mute ON") {
            // Simula um clique dentro dos limites do botão (e.g., 110, 110)
            event.mouse.x = 110;
            event.mouse.y = 110;

            button.processEvent(event);

            // Verifica se o método para mutar foi chamado
            CHECK(mock_sound_manager.mute_was_called);
            CHECK_FALSE(mock_sound_manager.unmute_was_called);
        }

        SUBCASE("Clicking inside the button again toggles mute OFF") {
            // 1. Muta o botão primeiro
            button.setMuted(true);
            mock_sound_manager.reset();

            // 2. Simula o segundo clique
            event.mouse.x = 125;
            event.mouse.y = 125;
            button.processEvent(event);

            // 3. Verifica se o método para desmutar foi chamado
            CHECK(mock_sound_manager.unmute_was_called);
            CHECK_FALSE(mock_sound_manager.mute_was_called);
        }

        SUBCASE("Clicking outside the button does nothing") {
            // Simula um clique fora dos limites (e.g., 10, 10)
            event.mouse.x = 10;
            event.mouse.y = 10;

            button.processEvent(event);

            // Verifica que nada aconteceu
            CHECK_FALSE(mock_sound_manager.mute_was_called);
            CHECK_FALSE(mock_sound_manager.unmute_was_called);
        }

        SUBCASE("Processing a non-mouse-click event does nothing") {
            event.type = ALLEGRO_EVENT_MOUSE_AXES; // Qualquer outro tipo de evento
            event.mouse.x = 110; // Dentro do botão
            event.mouse.y = 110;

            button.processEvent(event);

            // Verifica que nada aconteceu
            CHECK_FALSE(mock_sound_manager.mute_was_called);
            CHECK_FALSE(mock_sound_manager.unmute_was_called);
        }
    }
}