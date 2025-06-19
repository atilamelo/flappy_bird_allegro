#include "util/ResourceManager.hpp"

// Move neste caso é necessário para garantir que o ponteiro único é transferido corretamente
// e não duplicado, o que causaria um erro de gerenciamento de memória (já que estamos usando unique_ptr).

void ResourceManager::loadBitmap(const std::string& id, const std::string& filename) {
    BitmapPtr bitmap(al_load_bitmap(filename.c_str()));
    if (!bitmap) throw std::runtime_error("Falha em carregar o bitmap: " + filename);
    m_bitmaps.emplace(id, std::move(bitmap));
}

void ResourceManager::loadSubBitmap(const std::string& id, const std::string& source_id, int x, int y, int w, int h) {
    ALLEGRO_BITMAP* source_bitmap = getBitmap(source_id);
    BitmapPtr sub_bitmap(al_create_sub_bitmap(source_bitmap, x, y, w, h));
    if (!sub_bitmap) throw std::runtime_error("Falha em carregar o sub-bitmap: " + id);
    m_bitmaps.emplace(id, std::move(sub_bitmap));
}

ALLEGRO_BITMAP* ResourceManager::getBitmap(const std::string& id) const {
    auto it = m_bitmaps.find(id);
    if (it != m_bitmaps.end()) return it->second.get();
    throw std::runtime_error("Bitmap não encontrado: " + id);
}

void ResourceManager::loadSample(const std::string& id, const std::string& filename) {
    SamplePtr sample(al_load_sample(filename.c_str()));
    if (!sample) throw std::runtime_error("Falha em carregador a sample: " + filename);
    m_samples.emplace(id, std::move(sample));
}

ALLEGRO_SAMPLE* ResourceManager::getSample(const std::string& id) const {
    auto it = m_samples.find(id);
    if (it != m_samples.end()) return it->second.get();
    throw std::runtime_error("Sample não encontrada: " + id);
}
