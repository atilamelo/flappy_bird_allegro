#pragma once
#include <allegro5/allegro.h>

class Input {
public:
    static void init();
    static void update(ALLEGRO_EVENT* event);
    static bool isKeyDown(int keycode);
    static bool isKeyPressed(int keycode);

private:
    static unsigned char key[ALLEGRO_KEY_MAX];
    static constexpr unsigned char KEY_SEEN = 1;
    static constexpr unsigned char KEY_DOWN = 2;
};
