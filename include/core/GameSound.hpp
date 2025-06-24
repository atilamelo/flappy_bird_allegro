/**
 * @file GameSound.hpp
 * @brief Declaração da classe GameSound para gerenciamento global de efeitos sonoros e música de fundo.
 */

#pragma once
#include <string>
#include <allegro5/allegro_audio.h>

class GameSound {
public:
    GameSound();
    ~GameSound();

    /**
     * Inicializa os sons do jogo carregando a música de fundo específica do tema selecionado
     * e os efeitos sonoros padrão.
     * @param music_path Caminho para o arquivo de música de fundo do tema.
     */
    void init(const std::string& music_path);

    void play_background();
    void play_point();
    void play_death();
    void play_fly();
    void play_hit();

    void mute_music();
    void unmute_music();
    bool isMusicMuted() const;

    /**
     * Troca a música de fundo durante o jogo, destruindo a anterior se necessário.
     * @param music_path Caminho para o novo arquivo de música.
     */
    void setBackgroundMusic(const std::string& music_path);

private:
    ALLEGRO_AUDIO_STREAM* back_music;
    ALLEGRO_SAMPLE* point_sound;
    ALLEGRO_SAMPLE* died_sound;
    ALLEGRO_SAMPLE* hit_sound;
    ALLEGRO_SAMPLE* fly_sound;
    bool active_sound;
    bool music_muted;
};