#include "managers/ResourceManager.hpp"
#include "util/json.hpp"
#include <iostream>
#include <fstream>

// Move neste caso é necessário para garantir que o ponteiro único é transferido corretamente
// e não duplicado, o que causaria um erro de gerenciamento de memória (já que estamos usando unique_ptr).

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}   

void ResourceManager::loadBitmap(const std::string &id, const std::string &filename)
{
    BitmapPtr bitmap(al_load_bitmap(filename.c_str()));
    if (!bitmap)
        throw std::runtime_error("Falha em carregar o bitmap: " + filename);
    m_bitmaps.emplace(id, std::move(bitmap));
}

void ResourceManager::loadSubBitmap(const std::string &id, const std::string &source_id, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *source_bitmap = getBitmap(source_id);
    BitmapPtr sub_bitmap(al_create_sub_bitmap(source_bitmap, x, y, w, h));
    if (!sub_bitmap)
        throw std::runtime_error("Falha em carregar o sub-bitmap: " + id);
    m_bitmaps.emplace(id, std::move(sub_bitmap));
}

ALLEGRO_BITMAP *ResourceManager::getBitmap(const std::string &id) const
{
    auto it = m_bitmaps.find(id);
    if (it != m_bitmaps.end())
        return it->second.get();
    throw std::runtime_error("Bitmap não encontrado: " + id);
}

ALLEGRO_SAMPLE* ResourceManager::loadSample(const std::string &id, const std::string &filename)
{
    SamplePtr sample(al_load_sample(filename.c_str()));
    if (!sample)
        throw std::runtime_error("Falha em carregador a sample: " + filename);
    
    ALLEGRO_SAMPLE* samplePtr = sample.get();

    m_samples.emplace(id, std::move(sample));
    
    return samplePtr;
}

ALLEGRO_SAMPLE *ResourceManager::getSample(const std::string &id) const
{
    auto it = m_samples.find(id);
    if (it != m_samples.end())
        return it->second.get();
    throw std::runtime_error("Sample não encontrada: " + id);
}

void ResourceManager::loadAtlasJson(const std::string &json_filepath, const std::string &atlas_id, const std::string &main_atlas_sprite_sheet)
{
    try
    {
        getBitmap(atlas_id);
    }
    catch (const std::runtime_error &)
    {
        loadBitmap(atlas_id, main_atlas_sprite_sheet);
        std::cout << "Sprite sheet principal carregado: " << atlas_id << std::endl;
    }

    nlohmann::json json_data;
    std::ifstream json_file(json_filepath);

    if (!json_file.is_open())
    {
        throw std::runtime_error("Falha ao abrir o arquivo JSON do atlas: " + json_filepath);
    }

    try
    {
        json_file >> json_data;
        json_file.close();
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw std::runtime_error("Erro ao analisar o JSON: " + std::string(e.what()));
    }

    if (json_data.contains("sprites") && json_data["sprites"].is_array())
    {
        for (const auto &sprite_entry : json_data["sprites"])
        {
            try
            {
                std::string fileName = sprite_entry.at("fileName").get<std::string>();
                int width = sprite_entry.at("width").get<int>();
                int height = sprite_entry.at("height").get<int>();
                int x = sprite_entry.at("x").get<int>();
                int y = sprite_entry.at("y").get<int>();

                loadSubBitmap(fileName, atlas_id, x, y, width, height);
                std::cout << "Sub-bitmap '" << fileName << "' criado (x:" << x << ", y:" << y << ", w:" << width << ", h:" << height << ")." << std::endl;
            }
            catch (const nlohmann::json::exception &e)
            {
                std::cerr << "Erro ao ler dados de sprite no JSON '" << json_filepath << "': " << e.what() << std::endl;
                throw std::runtime_error("Erro ao ler dados de sprite no JSON '" + json_filepath + "': " + e.what());
            }
        }
    }
}
