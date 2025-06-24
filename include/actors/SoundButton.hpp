/**
 * @file SoundButton.hpp
 * @brief Declaração da classe SoundButton, um botão gráfico para mutar/desmutar a música do jogo.
 */

#pragma once

#include <allegro5/allegro.h>
#include "core/GameSound.hpp"
#include "interfaces/IDrawable.hpp"

/**
 * @brief Botão para controlar o mute da música de fundo do jogo.
 *
 * Mostra uma imagem diferente dependendo do estado (mutado/desmutado) e interage com o GameSound.
 */
class SoundButton : public IDrawable {
public:
    /**
     * @brief Construtor da classe SoundButton.
     * @param x Posição X do botão na tela.
     * @param y Posição Y do botão na tela.
     * @param w Largura do botão.
     * @param h Altura do botão.
     * @param img_on Ponteiro para bitmap da imagem "som ligado".
     * @param img_off Ponteiro para bitmap da imagem "som desligado".
     * @param soundManager Ponteiro para o gerenciador de som do jogo.
     */
    SoundButton(float x, float y, float w, float h, ALLEGRO_BITMAP* img_on, ALLEGRO_BITMAP* img_off, GameSound* soundManager);

    /**
     * @brief Desenha o botão na tela, exibindo a imagem correspondente ao estado de mute.
     */
    void draw() const override;

    /**
     * @brief Processa eventos de mouse para alternar o estado de mute.
     * @param event Evento do Allegro a ser processado.
     */
    void processEvent(const ALLEGRO_EVENT& event);

    /**
     * @brief Define se a música está mutada ou não.
     * @param value true para mutar, false para desmutar.
     */
    void setMuted(bool value);

private:
    float x, y, width, height; ///< Posição e dimensões do botão na tela.
    ALLEGRO_BITMAP* img_on;    ///< Imagem a ser exibida quando o som está ativo.
    ALLEGRO_BITMAP* img_off;   ///< Imagem a ser exibida quando o som está mutado.
    bool muted = false;        ///< Estado atual do som (mutado ou não).
    GameSound* soundManager;   ///< Ponteiro para o gerenciador de som do jogo.
};