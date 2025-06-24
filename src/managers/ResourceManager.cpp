#include "managers/ResourceManager.hpp"
#include "util/json.hpp"
#include <iostream>
#include <fstream>

// A gente usa std::move pra transferir a "posse" do ponteiro para o nosso mapa (m_bitmaps).
// Como é um unique_ptr, ele só pode ter um dono, e com o move a gente garante que o dono
// agora é o ResourceManager, que vai se encarregar de liberar a memória depois.

/**
 * @brief Retorna a instância única do gerenciador.
 * @details É um Singleton, então todo mundo no jogo usa essa mesma instância para acessar os recursos.
 */
ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance; // Criada só uma vez e vive até o fim do programa.
    return instance;
}

/**
 * @brief Carrega uma imagem (bitmap) de um arquivo e guarda ela com um apelido (ID).
 */
void ResourceManager::loadBitmap(const std::string &id, const std::string &filename)
{
    // Tenta carregar a imagem do arquivo.
    BitmapPtr bitmap(al_load_bitmap(filename.c_str()));
    if (!bitmap)
        throw std::runtime_error("Falha em carregar o bitmap: " + filename);
    
    // Se deu certo, guarda no nosso mapa de bitmaps.
    m_bitmaps.emplace(id, std::move(bitmap));
}

/**
 * @brief Cria uma "fatia" (sub-bitmap) de uma imagem maior que já foi carregada.
 */
void ResourceManager::loadSubBitmap(const std::string &id, const std::string &source_id, int x, int y, int w, int h)
{
    // Primeiro, pega a imagem "mãe".
    ALLEGRO_BITMAP *source_bitmap = getBitmap(source_id);
    // Agora, cria o recorte.
    BitmapPtr sub_bitmap(al_create_sub_bitmap(source_bitmap, x, y, w, h));
    if (!sub_bitmap)
        throw std::runtime_error("Falha em carregar o sub-bitmap: " + id);
    
    // Guarda o recorte no nosso mapa.
    m_bitmaps.emplace(id, std::move(sub_bitmap));
}

/**
 * @brief Pega um ponteiro para uma imagem já carregada, usando o apelido (ID) dela.
 */
ALLEGRO_BITMAP *ResourceManager::getBitmap(const std::string &id) const
{
    auto it = m_bitmaps.find(id);
    if (it != m_bitmaps.end())
        return it->second.get(); // .get() pega o ponteiro bruto de dentro do smart pointer.
    throw std::runtime_error("Bitmap não encontrado: " + id);
}

/**
 * @brief Carrega um efeito sonoro (sample) de um arquivo.
 */
ALLEGRO_SAMPLE* ResourceManager::loadSample(const std::string &id, const std::string &filename)
{
    SamplePtr sample(al_load_sample(filename.c_str()));
    if (!sample)
        throw std::runtime_error("Falha em carregador a sample: " + filename);
    
    // Pega o ponteiro bruto pra retornar pra quem pediu.
    ALLEGRO_SAMPLE* samplePtr = sample.get();
    
    // Guarda o sample no mapa (o gerenciador continua sendo o dono).
    m_samples.emplace(id, std::move(sample));
    
    return samplePtr;
}

/**
 * @brief Pega um ponteiro para um efeito sonoro já carregado.
 */
ALLEGRO_SAMPLE *ResourceManager::getSample(const std::string &id) const
{
    auto it = m_samples.find(id);
    if (it != m_samples.end())
        return it->second.get();
    throw std::runtime_error("Sample não encontrada: " + id);
}

/**
 * @brief Carrega uma música (stream) de um arquivo. Streams são bons para áudios longos.
 */
ALLEGRO_AUDIO_STREAM* ResourceManager::loadAudioStream(const std::string& id, const std::string& filename, size_t buffer_count, size_t samples) {
    auto stream = AudioStreamPtr(al_load_audio_stream(filename.c_str(), buffer_count, samples));
    if (!stream) {
        throw std::runtime_error("Falha ao carregar o stream de áudio: " + filename);
    }

    // Mesma ideia do loadSample: pega o ponteiro bruto pra retornar...
    ALLEGRO_AUDIO_STREAM* streamPtr = stream.get();
    // ...mas o gerenciador guarda o smart pointer e continua sendo o dono.
    m_audio_streams[id] = std::move(stream);
    
    return streamPtr;
}

/**
 * @brief Pega um ponteiro para uma música (stream) já carregada.
 */
ALLEGRO_AUDIO_STREAM* ResourceManager::getAudioStream(const std::string& id) const {
    auto it = m_audio_streams.find(id);
    if (it == m_audio_streams.end()) {
        throw std::runtime_error("Stream de áudio não encontrado: " + id);
    }
    return it->second.get();
}

/**
 * @brief Carrega um atlas de sprites a partir de um arquivo JSON.
 * @details Um atlas é uma imagem grande (sprite sheet) com vários sprites menores dentro.
 * O arquivo JSON serve como um "mapa", dizendo onde cada sprite pequeno está.
 */
void ResourceManager::loadAtlasJson(const std::string &json_filepath, const std::string &atlas_id, const std::string &main_atlas_sprite_sheet)
{
    // Checa se a imagem principal (o sprite sheet) já foi carregada. Se não, carrega agora.
    try
    {
        getBitmap(atlas_id);
    }
    catch (const std::runtime_error &)
    {
        loadBitmap(atlas_id, main_atlas_sprite_sheet);
        std::cout << "Sprite sheet principal carregado: " << atlas_id << std::endl;
    }

    // Agora, vamos ler o arquivo JSON que tem as coordenadas de cada sprite.
    nlohmann::json json_data;
    std::ifstream json_file(json_filepath);

    if (!json_file.is_open())
    {
        throw std::runtime_error("Falha ao abrir o arquivo JSON do atlas: " + json_filepath);
    }

    // Tenta "entender" (parse) o conteúdo do arquivo JSON.
    try
    {
        json_file >> json_data;
        json_file.close();
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw std::runtime_error("Erro ao analisar o JSON: " + std::string(e.what()));
    }

    // Se o JSON tiver uma lista chamada "sprites", a gente passa por cada item dela.
    if (json_data.contains("sprites") && json_data["sprites"].is_array())
    {
        for (const auto &sprite_entry : json_data["sprites"])
        {
            try
            {
                // Para cada item, pega o nome, tamanho e posição.
                std::string fileName = sprite_entry.at("fileName").get<std::string>();
                int width = sprite_entry.at("width").get<int>();
                int height = sprite_entry.at("height").get<int>();
                int x = sprite_entry.at("x").get<int>();
                int y = sprite_entry.at("y").get<int>();

                // E usa essas informações pra criar um sub-bitmap (um recorte da imagem principal).
                loadSubBitmap(fileName, atlas_id, x, y, width, height);
            }
            catch (const nlohmann::json::exception &e)
            {
                std::cerr << "Erro ao ler dados de sprite no JSON '" << json_filepath << "': " << e.what() << std::endl;
                throw std::runtime_error("Erro ao ler dados de sprite no JSON '" + json_filepath + "': " + e.what());
            }
        }
    }
}