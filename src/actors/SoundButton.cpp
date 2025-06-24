#include "actors/SoundButton.hpp"
#include <iostream>

SoundButton::SoundButton(float x, float y, float w, float h, ALLEGRO_BITMAP* img_on, ALLEGRO_BITMAP* img_off, GameSound* soundManager)
    : x(x), y(y), width(w), height(h), img_on(img_on), img_off(img_off), soundManager(soundManager) {}

void SoundButton::draw() const {
    ALLEGRO_BITMAP* img = muted ? img_off : img_on;
    if (img)
        al_draw_scaled_bitmap(img, 0, 0,
            al_get_bitmap_width(img), al_get_bitmap_height(img),
            x, y, width, height, 0);
}

void SoundButton::processEvent(const ALLEGRO_EVENT& event) {
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = event.mouse.x;
        float my = event.mouse.y;
        if (mx >= x && mx <= x + width && my >= y && my <= y + height) {
            muted = !muted;
            setMuted(muted);
        }
    }
}

void SoundButton::setMuted(bool value) {
    muted = value;
   if (soundManager) {
    if (muted)
        soundManager->mute_music();
    else
        soundManager->unmute_music();
}
}