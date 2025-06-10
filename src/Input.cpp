#include "Input.hpp"
#include <cstring>

// Quantidade de teclas que o Allegro suporta
unsigned char Input::key[ALLEGRO_KEY_MAX];

void Input::init() {
    memset(key, 0, sizeof(key));
}

// Usa máscara de bits para armazenar o estado das teclas
void Input::update(ALLEGRO_EVENT* event) {
    switch (event->type) {
        case ALLEGRO_EVENT_TIMER:
            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= ~KEY_SEEN; // Limpa o estado de "visto" de todas as teclas
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_DOWN; // A tecla foi pressionada e está sendo mantida
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= ~KEY_DOWN; // A tecla foi solta
            break;
    }
}

bool Input::isKeyDown(int keycode) {
    return key[keycode] & KEY_DOWN;
}

bool Input::isKeyPressed(int keycode) {
    return key[keycode] & KEY_SEEN;
}
