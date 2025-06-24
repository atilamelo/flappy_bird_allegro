/**
 * @file SoundButton.cpp
 * @brief Implementação da classe SoundButton.
 */

#include "actors/SoundButton.hpp"
#include <allegro5/allegro.h>
#include <iostream>

/**
 * @brief Inicializa o botão de som com posição, tamanho, imagens e referência ao gerenciador de som.
 */
SoundButton::SoundButton(float x, float y, float w, float h, ALLEGRO_BITMAP* img_on, ALLEGRO_BITMAP* img_off, GameSound* soundManager)
    : x(x), y(y), width(w), height(h), img_on(img_on), img_off(img_off), soundManager(soundManager), muted(false) {}

/**
 * @brief Desenha a imagem correspondente ao estado de mute.
 * Se o som estiver mutado, desenha img_off; caso contrário, desenha img_on.
 */
void SoundButton::draw() const {
    ALLEGRO_BITMAP* img = muted ? img_off : img_on;
    if (img) {
        // Desenha o bitmap escalonado para caber nas dimensões do botão
        al_draw_scaled_bitmap(
            img,
            0, 0,
            al_get_bitmap_width(img), al_get_bitmap_height(img),
            x, y, width, height, 0
        );
    }
}

/**
 * @brief Processa evento de clique do mouse.
 * Se o botão for clicado, alterna o estado de mute e atualiza o som.
 */
void SoundButton::processEvent(const ALLEGRO_EVENT& event) {
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = event.mouse.x;
        float my = event.mouse.y;
        // Verifica se o clique ocorreu dentro dos limites do botão
        if (mx >= x && mx <= x + width && my >= y && my <= y + height) {
            setMuted(!muted);
        }
    }
}

/**
 * @brief Define o estado de mute e aplica ao gerenciador de som.
 * @param value true para mutar, false para desmutar.
 */
void SoundButton::setMuted(bool value) {
    muted = value;
    if (soundManager) {
        if (muted)
            soundManager->mute_music();
        else
            soundManager->unmute_music();
    }
}