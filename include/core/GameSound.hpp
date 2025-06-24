#pragma once
#include <allegro5/allegro_audio.h>

class GameSound {
public:
    GameSound();
    ~GameSound();

    void init();               
    void play_background();       
    void play_point();         
    void play_death(); 
    void play_fly();         
    void play_hit();          
    void mute_music();
    void unmute_music();
    bool isMusicMuted() const;

private:
    ALLEGRO_AUDIO_STREAM* back_music = nullptr;
    ALLEGRO_SAMPLE* point_sound = nullptr;
    ALLEGRO_SAMPLE* died_sound = nullptr;
    ALLEGRO_SAMPLE* fly_sound = nullptr;
    ALLEGRO_SAMPLE* hit_sound = nullptr;
    bool active_sound;
    bool music_muted = false;
};