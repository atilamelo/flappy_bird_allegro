#include "core/GameSound.hpp"
#include <allegro5/allegro_acodec.h>
#include <iostream>

GameSound::GameSound() {}
GameSound::~GameSound() {
    if (back_instance) al_destroy_sample_instance(back_instance);
    if (back_sound) al_destroy_sample(back_sound);
    if (point_sound) al_destroy_sample(point_sound);
    if (died_sound) al_destroy_sample(died_sound);
}

void GameSound::init() {
    back_sound = al_load_sample("assets/audio/8bit.ogg");
    if (!back_sound) std::cerr << "Erro ao carregar som de fundo\n";
    else {
        back_instance = al_create_sample_instance(back_sound);
        if (!back_instance) std::cerr << "Erro ao criar instância de som de fundo\n";
        else {
            al_attach_sample_instance_to_mixer(back_instance, al_get_default_mixer());
            al_set_sample_instance_playmode(back_instance, ALLEGRO_PLAYMODE_LOOP);
            al_set_sample_instance_gain(back_instance, 0.8f);
        }
        if (active_sound && back_instance) al_set_sample_instance_playing(back_instance, true);
    }
    point_sound = al_load_sample("assets/audio/point.wav");
    if (!point_sound) std::cerr << "Erro ao carregar som de ponto\n";
    died_sound = al_load_sample("assets/audio/die.wav");
    if (!died_sound) std::cerr << "Erro ao carregar som de morte\n";
}

void GameSound::play_background() { if (back_instance && active_sound) al_set_sample_instance_playing(back_instance, true); }
void GameSound::play_point() { if (point_sound && active_sound) al_play_sample(point_sound, 1,0,1,ALLEGRO_PLAYMODE_ONCE,nullptr);}
void GameSound::play_death() { if (died_sound && active_sound) al_play_sample(died_sound, 1,0,1,ALLEGRO_PLAYMODE_ONCE,nullptr);}
void GameSound::pause_all() { if (back_instance) al_set_sample_instance_playing(back_instance, false); }
void GameSound::resume_all() { if (back_instance && active_sound) al_set_sample_instance_playing(back_instance, true); }
void GameSound::setActive(bool active) { active_sound = active; if (active) resume_all(); else pause_all(); }
bool GameSound::isActive() const { return active_sound; }