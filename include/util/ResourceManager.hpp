#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

// -- Deleters para Smarts Pointers ---
struct AllegroBitmapDeleter { void operator()(ALLEGRO_BITMAP* bmp) const { if (bmp) al_destroy_bitmap(bmp); } };
struct AllegroFontDeleter { void operator()(ALLEGRO_FONT* font) const { if (font) al_destroy_font(font); } };
struct AllegroSampleDeleter { void operator()(ALLEGRO_SAMPLE* sample) const { if (sample) al_destroy_sample(sample); } };
    
// --- Apelidos para Smarts Pointers  ---
using BitmapPtr = std::unique_ptr<ALLEGRO_BITMAP, AllegroBitmapDeleter>;
using FontPtr = std::unique_ptr<ALLEGRO_FONT, AllegroFontDeleter>;
using SamplePtr = std::unique_ptr<ALLEGRO_SAMPLE, AllegroSampleDeleter>;

class ResourceManager {
public:
    void loadBitmap(const std::string& id, const std::string& filename);
    void loadSubBitmap(const std::string& id, const std::string& source_id, int x, int y, int w, int h);
    void loadSample(const std::string& id, const std::string& filename);
    void loadAtlasJson(const std::string& json_filepath, const std::string& atlas_id, const std::string& main_sprite_sheet_filepath);

    ALLEGRO_BITMAP* getBitmap(const std::string& id) const;
    ALLEGRO_SAMPLE* getSample(const std::string& id) const;

private:
    std::map<std::string, BitmapPtr> m_bitmaps;
    std::map<std::string, SamplePtr> m_samples;
};