/**
 * @file GameSound.cpp
 * @brief Implementação da classe GameSound para gerenciar sons do jogo, incluindo seleção dinâmica de música de fundo por tema.
 */

#include "core/GameSound.hpp"
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include "managers/ResourceManager.hpp"

/**
 * @brief Construtor padrão: inicializa ponteiros e flags.
 */
GameSound::GameSound()
    : back_music(nullptr), point_sound(nullptr), died_sound(nullptr), hit_sound(nullptr), fly_sound(nullptr), active_sound(true), music_muted(false)
{}


/**
 * @brief Inicializa a música de fundo e os efeitos do jogo, usando ResourceManager.
 * @param music_name Caminho para a trilha de fundo.
 */
void GameSound::init(const std::string& music_name) {
    ResourceManager& rm = ResourceManager::getInstance();

    // Carrega a música de fundo do tema como ALLEGRO_AUDIO_STREAM
    back_music = rm.getAudioStream(music_name);
    al_set_audio_stream_playmode(back_music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(back_music, al_get_default_mixer());
    al_set_audio_stream_gain(back_music, 0.10f); // Volume padrão
    al_set_audio_stream_playing(back_music, true);
    
    // Carrega os sons de evento como ALLEGRO_SAMPLE
    point_sound = rm.getSample("point");
    died_sound = rm.getSample("die");
    hit_sound = rm.getSample("hit");
    fly_sound = rm.getSample("fly");
}

/**
 * @brief Inicia a reprodução da música de fundo, se o som estiver ativo.
 */
void GameSound::play_background() {
    if (back_music && active_sound) {
        al_set_audio_stream_playing(back_music, true);
    }
}

/**
 * @brief Reproduz o efeito de ponto, se o som estiver ativo.
 */
void GameSound::play_point() {
    if (point_sound && active_sound) {
        al_play_sample(point_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

/**
 * @brief Reproduz o efeito sonoro de morte, se o som estiver ativo.
 */
void GameSound::play_death() {
    if (died_sound && active_sound) {
        al_play_sample(died_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

/**
 * @brief Reproduz o efeito sonoro de voo, se o som estiver ativo.
 */
void GameSound::play_fly() {
    if (fly_sound && active_sound) {
        al_play_sample(fly_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

/**
 * @brief Reproduz o efeito sonoro de batida, se o som estiver ativo.
 */
void GameSound::play_hit() {
    if (hit_sound && active_sound) {
        al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

/**
 * @brief Muta apenas a música de fundo (volume para 0).
 */
void GameSound::mute_music() {
    if (back_music) {
        al_set_audio_stream_playing(back_music, false);
        music_muted = true;
    }
}

/**
 * @brief Desmuta apenas a música de fundo (volume padrão).
 */
void GameSound::unmute_music() {
    if (back_music) {
        al_set_audio_stream_playing(back_music, true);
        music_muted = false;
    }
}

/**
 * @brief Informa se a música está mutada.
 * @return true se a música está mutada, false caso contrário.
 */
bool GameSound::isMusicMuted() const {
    return music_muted;
}

/**
 * @brief Troca a música de fundo durante o jogo, destruindo a anterior se necessário.
 * @param music_name Caminho para o novo arquivo de música.
 */
void GameSound::setBackgroundMusic(const std::string& music_name) {
    if (back_music) {
        al_destroy_audio_stream(back_music);
    }
    ResourceManager& rm = ResourceManager::getInstance();
    back_music = rm.loadAudioStream("musica_tema", music_name, 4, 4096);
    if (back_music) {
        al_set_audio_stream_playmode(back_music, ALLEGRO_PLAYMODE_LOOP);
        al_attach_audio_stream_to_mixer(back_music, al_get_default_mixer());
        al_set_audio_stream_gain(back_music, 0.5f);
        al_set_audio_stream_playing(back_music, true);
    }
}