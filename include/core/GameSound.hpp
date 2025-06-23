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
    void pause_all();           
    void resume_all();
    void setActive(bool active);
    bool isActive() const;

private:
    ALLEGRO_SAMPLE* back_sound = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* back_instance = nullptr;
    ALLEGRO_SAMPLE* point_sound = nullptr;
    ALLEGRO_SAMPLE* died_sound = nullptr;
    bool active_sound = true;
};