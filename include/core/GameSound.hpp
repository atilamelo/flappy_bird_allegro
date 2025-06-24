/**
 * @file GameSound.hpp
 * @brief Declaração da classe GameSound para gerenciamento global de efeitos sonoros e música de fundo.
 */

#pragma once
#include <string>
#include <allegro5/allegro_audio.h>

/**
 * @brief Classe responsável por gerenciar todos os sons do jogo (música e efeitos).
 */
class GameSound {
public:
    GameSound();
    ~GameSound();

    /**
     * @brief Inicializa os sons do jogo, carregando música de fundo e efeitos.
     * @param music_path Caminho para o arquivo de música de fundo.
     */
    void init(const std::string& music_path);

    /// Reproduz a música de fundo, se ativa.
    void play_background();

    /// Reproduz o efeito sonoro de ponto.
    void play_point();

    /// Reproduz o efeito sonoro de morte.
    void play_death();

    /// Reproduz o efeito sonoro de voo.
    void play_fly();

    /// Reproduz o efeito sonoro de batida.
    void play_hit();

    /// Muta a música de fundo.
    void mute_music();

    /// Desmuta a música de fundo.
    void unmute_music();

    /// Retorna se a música está mutada.
    bool isMusicMuted() const;

    /**
     * @brief Troca a música de fundo durante o jogo, destruindo a anterior se necessário.
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