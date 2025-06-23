#pragma once
#include <allegro5/allegro.h>
#include "core/GameSound.hpp"
#include "interfaces/IDrawable.hpp"

class SoundButton : public IDrawable {
public:
    SoundButton(float x, float y, float w, float h, ALLEGRO_BITMAP* img_on, ALLEGRO_BITMAP* img_off, GameSound* soundManager);

    void draw() const override;
    void processEvent(const ALLEGRO_EVENT& event);
    void setMuted(bool value);

private:
    float x, y, width, height;
    ALLEGRO_BITMAP* img_on;
    ALLEGRO_BITMAP* img_off;
    bool muted = false;
    GameSound* soundManager; // ponteiro para o gerenciador de som
};