#include "core/GameSound.hpp"
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include "managers/ResourceManager.hpp"
GameSound::GameSound()
    : back_music(nullptr), point_sound(nullptr), died_sound(nullptr), active_sound(true)
{}

GameSound::~GameSound() {
    if (back_music) {
        al_set_audio_stream_playing(back_music, false);
    }
}

void GameSound::init() {
    ResourceManager& rm = ResourceManager::getInstance();

    // Carrega a música de fundo como ALLEGRO_AUDIO_STREAM
    back_music = rm.loadAudioStream("8bit", "assets/audio/8bit.ogg", 2, 2048);
    if (!back_music) {
        std::cerr << "Erro ao carregar stream de música de fundo: 8bit.ogg\n";
    } else {
        al_set_audio_stream_playmode(back_music, ALLEGRO_PLAYMODE_LOOP);
        al_attach_audio_stream_to_mixer(back_music, al_get_default_mixer());
        al_set_audio_stream_gain(back_music, 0.5f);
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

bool music_muted = false;

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
        al_set_audio_stream_gain(back_music, 0.05f); 
        music_muted = false;
    }
}

bool GameSound::isMusicMuted() const {
    return music_muted;
}