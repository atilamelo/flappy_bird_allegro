#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

// -- Deleters para Smart Pointers ---

/** @struct AllegroBitmapDeleter
 * @brief Functor para destruir um ALLEGRO_BITMAP, para uso com std::unique_ptr. */
struct AllegroBitmapDeleter { void operator()(ALLEGRO_BITMAP* bmp) const { if (bmp) al_destroy_bitmap(bmp); } };

/** @struct AllegroFontDeleter
 * @brief Functor para destruir um ALLEGRO_FONT, para uso com std::unique_ptr. */
struct AllegroFontDeleter { void operator()(ALLEGRO_FONT* font) const { if (font) al_destroy_font(font); } };

/** @struct AllegroSampleDeleter
 * @brief Functor para destruir um ALLEGRO_SAMPLE, para uso com std::unique_ptr. */
struct AllegroSampleDeleter { void operator()(ALLEGRO_SAMPLE* sample) const { if (sample) al_destroy_sample(sample); } };

/** @struct AllegroAudioStreamDeleter
 * @brief Functor para destruir um ALLEGRO_AUDIO_STREAM, para uso com std::unique_ptr. */
struct AllegroAudioStreamDeleter { void operator()(ALLEGRO_AUDIO_STREAM* stream) const { if (stream) al_destroy_audio_stream(stream); } };


// --- Apelidos para Smart Pointers ---

/** @typedef BitmapPtr
 * @brief Apelido para um ponteiro inteligente que gerencia o ciclo de vida de um ALLEGRO_BITMAP. */
using BitmapPtr = std::unique_ptr<ALLEGRO_BITMAP, AllegroBitmapDeleter>;

/** @typedef FontPtr
 * @brief Apelido para um ponteiro inteligente que gerencia o ciclo de vida de um ALLEGRO_FONT. */
using FontPtr = std::unique_ptr<ALLEGRO_FONT, AllegroFontDeleter>;

/** @typedef SamplePtr
 * @brief Apelido para um ponteiro inteligente que gerencia o ciclo de vida de um ALLEGRO_SAMPLE. */
using SamplePtr = std::unique_ptr<ALLEGRO_SAMPLE, AllegroSampleDeleter>;

/** @typedef AudioStreamPtr
 * @brief Apelido para um ponteiro inteligente que gerencia o ciclo de vida de um ALLEGRO_AUDIO_STREAM. */
using AudioStreamPtr = std::unique_ptr<ALLEGRO_AUDIO_STREAM, AllegroAudioStreamDeleter>;


/**
 * @file ResourceManager.hpp
 * @brief Define a classe ResourceManager, um singleton para gerenciar o carregamento e o acesso a recursos do jogo.
 * @details Este gerenciador centraliza todos os recursos como texturas e sons, evitando duplicação de
 * carregamento e simplificando o gerenciamento de memória através de ponteiros inteligentes.
 */

/**
 * @class ResourceManager
 * @brief Gerenciador de recursos do jogo implementado como um Singleton.
 * @details Garante que apenas uma instância desta classe exista, provendo um ponto de
 * acesso global para todos os assets (bitmaps, samples, etc.). A memória dos recursos
 * é gerenciada automaticamente por std::unique_ptr com deleters customizados.
 */
class ResourceManager {
public:
    /**
     * @brief Obtém a instância única do ResourceManager.
     * @return Uma referência para a instância do ResourceManager.
     */
    static ResourceManager& getInstance();

    /**
     * @brief Carrega um bitmap a partir de um arquivo e o armazena com um ID.
     * @param id A chave de texto única para identificar este recurso.
     * @param filename O caminho do arquivo de imagem a ser carregado.
     * @throw std::runtime_error se o bitmap não puder ser carregado.
     */
    void loadBitmap(const std::string& id, const std::string& filename);

    /**
     * @brief Cria um sub-bitmap a partir de um bitmap já carregado.
     * @param id A nova chave de texto única para o sub-bitmap.
     * @param source_id O ID do bitmap original (fonte).
     * @param x A coordenada X do canto superior esquerdo no bitmap de origem.
     * @param y A coordenada Y do canto superior esquerdo no bitmap de origem.
     * @param w A largura do sub-bitmap.
     * @param h A altura do sub-bitmap.
     * @throw std::runtime_error se o bitmap de origem não existir.
     */
    void loadSubBitmap(const std::string& id, const std::string& source_id, int x, int y, int w, int h);

    /**
     * @brief Carrega uma amostra de áudio (sample) a partir de um arquivo.
     * @param id A chave de texto única para identificar este recurso.
     * @param filename O caminho do arquivo de áudio a ser carregado.
     * @return Um ponteiro bruto para o recurso carregado. Note que a posse continua com o ResourceManager.
     * @throw std::runtime_error se o sample não puder ser carregado.
     */
    ALLEGRO_SAMPLE* loadSample(const std::string& id, const std::string& filename);

    /**
     * @brief Carrega um stream de áudio a partir de um arquivo.
     * @param id A chave de texto única para identificar este recurso.
     * @param filename O caminho do arquivo de áudio a ser carregado.
     * @param buffer_count O número de buffers para o stream.
     * @param samples O número de amostras por buffer.
     * @return Um ponteiro bruto para o recurso carregado. A posse é mantida pelo ResourceManager.
     * @throw std::runtime_error se o stream de áudio não puder ser carregado.
     */
    ALLEGRO_AUDIO_STREAM* loadAudioStream(const std::string& id, const std::string& filename, size_t buffer_count, size_t samples);
    
    /**
     * @brief Carrega uma série de sub-bitmaps definidos em um arquivo JSON (atlas de texturas).
     * @param json_filepath O caminho para o arquivo JSON que define os sprites.
     * @param atlas_id Um prefixo para os IDs dos sprites carregados a partir do atlas.
     * @param main_sprite_sheet_filepath O caminho para a imagem principal (spritesheet) que o JSON referencia.
     */
    void loadAtlasJson(const std::string& json_filepath, const std::string& atlas_id, const std::string& main_sprite_sheet_filepath);

    /**
     * @brief Recupera um ponteiro para um bitmap previamente carregado.
     * @param id O ID do bitmap a ser recuperado.
     * @return Um ponteiro bruto para o ALLEGRO_BITMAP.
     * @throw std::runtime_error se o ID do bitmap não for encontrado.
     */
    ALLEGRO_BITMAP* getBitmap(const std::string& id) const;

    /**
     * @brief Recupera um ponteiro para um sample de áudio previamente carregado.
     * @param id O ID do sample a ser recuperado.
     * @return Um ponteiro bruto para o ALLEGRO_SAMPLE.
     * @throw std::runtime_error se o ID do sample não for encontrado.
     */
    ALLEGRO_SAMPLE* getSample(const std::string& id) const;

    /**
     * @brief Recupera um ponteiro para um stream de áudio previamente carregado.
     * @param id O ID do stream de áudio a ser recuperado.
     * @return Um ponteiro bruto para o ALLEGRO_AUDIO_STREAM.
     * @throw std::runtime_error se o ID do stream não for encontrado.
     */
    ALLEGRO_AUDIO_STREAM* getAudioStream(const std::string& id) const;

private:
    /**
     * @brief Construtor privado para garantir a implementação do padrão Singleton.
     */
    ResourceManager() = default;

    /**
     * @brief Construtor de cópia deletado para impedir a cópia da instância do Singleton.
     */
    ResourceManager(const ResourceManager&) = delete;

    /**
     * @brief Operador de atribuição deletado para impedir a atribuição da instância do Singleton.
     */
    ResourceManager& operator=(const ResourceManager&) = delete;

    /// @brief Mapa que armazena os bitmaps carregados, associando um ID a um ponteiro inteligente.
    std::map<std::string, BitmapPtr> m_bitmaps;
    
    /// @brief Mapa que armazena os samples de áudio, associando um ID a um ponteiro inteligente.
    std::map<std::string, SamplePtr> m_samples;
    
    /// @brief Mapa que armazena os streams de áudio, associando um ID a um ponteiro inteligente.
    std::map<std::string, AudioStreamPtr> m_audio_streams;
};