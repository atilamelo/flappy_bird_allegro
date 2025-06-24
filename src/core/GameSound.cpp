/**
 * @file GameSound.cpp
 * @brief Implementação da classe GameSound para gerenciar sons do jogo, incluindo seleção dinâmica de música de fundo por tema.
 */

#include "core/GameSound.hpp"
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include "managers/ResourceManager.hpp"

GameSound::GameSound()
    : back_music(nullptr), point_sound(nullptr), died_sound(nullptr), hit_sound(nullptr), fly_sound(nullptr), active_sound(true), music_muted(false)
{}

GameSound::~GameSound() {
    if (back_music) {
        al_destroy_audio_stream(back_music);
        back_music = nullptr;
    }
}

void GameSound::init(const std::string& music_path) {
    ResourceManager& rm = ResourceManager::getInstance();

    // Carrega a música de fundo do tema como ALLEGRO_AUDIO_STREAM
    if (back_music) {
        al_destroy_audio_stream(back_music);
    }
    back_music = rm.loadAudioStream("musica_tema", music_path, 4, 4096);
    if (!back_music) {
        std::cerr << "Erro ao carregar stream de música de fundo: " << music_path << std::endl;
    } else {
        al_set_audio_stream_playmode(back_music, ALLEGRO_PLAYMODE_LOOP);
        al_attach_audio_stream_to_mixer(back_music, al_get_default_mixer());
        al_set_audio_stream_gain(back_music, 0.5f);
        al_set_audio_stream_playing(back_music, true);
    }

    // Carrega os sons de evento como ALLEGRO_SAMPLE
    point_sound = rm.loadSample("point", "assets/audio/point.wav");
    if (!point_sound) {
        std::cerr << "Erro ao carregar som de ponto: point.wav\n";
    }

    died_sound = rm.loadSample("die", "assets/audio/die.wav");
    if (!died_sound) {
        std::cerr << "Erro ao carregar som de morte: die.wav\n";
    }

    hit_sound = rm.loadSample("hit", "assets/audio/hit.wav");
    if (!hit_sound) {
        std::cerr << "Erro ao carregar som de batida: hit.wav\n";
    }

    fly_sound = rm.loadSample("fly", "assets/audio/wing.wav");
    if (!fly_sound) {
        std::cerr << "Erro ao carregar som de voo: wing.wav\n";
    }
}

void GameSound::play_background() {
    if (back_music && active_sound) {
        al_set_audio_stream_playing(back_music, true);
    }
}

void GameSound::play_point() {
    if (point_sound && active_sound) {
        al_play_sample(point_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

void GameSound::play_death() {
    if (died_sound && active_sound) {
        al_play_sample(died_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

void GameSound::play_fly() {
    if (fly_sound && active_sound) {
        al_play_sample(fly_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

void GameSound::play_hit() {
    if (hit_sound && active_sound) {
        al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

// Método para mutar só a música de fundo:
void GameSound::mute_music() {
    if (back_music) {
        al_set_audio_stream_gain(back_music, 0.0f);
        music_muted = true;
    }
}

// Método para desmutar só a música de fundo:
void GameSound::unmute_music() {
    if (back_music) {
        al_set_audio_stream_gain(back_music, 0.5f); 
        music_muted = false;
    }
}

bool GameSound::isMusicMuted() const {
    return music_muted;
}

// Troca a música de fundo durante o jogo (por exemplo, ao mudar de tema)
void GameSound::setBackgroundMusic(const std::string& music_path) {
    if (back_music) {
        al_destroy_audio_stream(back_music);
    }
    ResourceManager& rm = ResourceManager::getInstance();
    back_music = rm.loadAudioStream("musica_tema", music_path, 4, 4096);
    if (back_music) {
        al_set_audio_stream_playmode(back_music, ALLEGRO_PLAYMODE_LOOP);
        al_attach_audio_stream_to_mixer(back_music, al_get_default_mixer());
        al_set_audio_stream_gain(back_music, 0.5f);
        al_set_audio_stream_playing(back_music, true);
    }
}