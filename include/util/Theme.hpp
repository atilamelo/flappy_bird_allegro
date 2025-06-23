/**
 * @file Theme.hpp
 * @brief Define a estrutura de dados para um tema visual do jogo.
 */
#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <vector>
#include <string>

/**
 * @struct Theme
 * @brief Agrupa todos os assets visuais e sonoros para um tema específico do jogo.
 *
 * Esta estrutura contém ponteiros diretos para os recursos já carregados pelo
 * ResourceManager, facilitando a passagem de um conjunto completo de assets
 * entre as cenas.
 */
struct Theme {
    std::string name;                          ///< Nome do tema (ex: "Amarelo", "Nerd").
    std::vector<ALLEGRO_BITMAP*> bird_frames;  ///< Frames de animação para o pássaro do tema.
    ALLEGRO_BITMAP* background;                ///< Bitmap do fundo do cenário.
    ALLEGRO_BITMAP* floor;                     ///< Bitmap da base/chão.
    ALLEGRO_BITMAP* pipe;                      ///< Bitmap do cano.
    ALLEGRO_AUDIO_STREAM* music;               ///< Amostra de áudio para a música de fundo do tema.
};