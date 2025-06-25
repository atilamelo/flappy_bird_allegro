/**
 * @file CharacterSelectionScene.cpp
 * @brief Implementação da cena de seleção de temas.
 */

#include "scenes/CharacterSelectionScene.hpp"
#include "scenes/GameScene.hpp"
#include "managers/SceneManager.hpp"
#include "managers/ResourceManager.hpp"
#include "Constants.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

CharacterSelectionScene::CharacterSelectionScene(SceneManager *sceneManager)
    : Scene(sceneManager), selectedIndex(0)
{
    buildThemes();
    font = al_create_builtin_font();
    selection_color = al_map_rgb(255, 255, 255);
}

CharacterSelectionScene::~CharacterSelectionScene()
{
    if (font)
    {
        al_destroy_font(font);
    }
}

void CharacterSelectionScene::buildThemes()
{
    ResourceManager &rm = ResourceManager::getInstance();

    // Adiciona os dados de cada tema, incluindo o caminho da música
    themes.push_back({
        "Amarelo",
        {rm.getBitmap("yellowbird-downflap"), rm.getBitmap("yellowbird-midflap"), rm.getBitmap("yellowbird-upflap")},
        rm.getBitmap("background-day"),
        rm.getBitmap("base"),
        rm.getBitmap("pipe-green"),
        "8bitMusicTheme"
    });

    themes.push_back({
        "Nerd",
        {rm.getBitmap("nerd_0"), rm.getBitmap("nerd_1"), rm.getBitmap("nerd_2")},
        rm.getBitmap("background-night"),
        rm.getBitmap("nerd_base"),
        rm.getBitmap("nerd_pipe"),
        "starMusicTheme"
    });

    themes.push_back({
        "Barbie",
        {rm.getBitmap("barbielaco_0"), rm.getBitmap("barbielaco_1"), rm.getBitmap("barbielaco_2")},
        rm.getBitmap("barbie_background"),
        rm.getBitmap("barbie_base"),
        rm.getBitmap("barbie_pipe"),
        "barbie"
    });

    themes.push_back({
        "Porco",
        {rm.getBitmap("pig_0"), rm.getBitmap("pig_1"), rm.getBitmap("pig_2")},
        rm.getBitmap("pig_background"),
        rm.getBitmap("pig_base"),
        rm.getBitmap("pig_pipe"),
        "yoshi"
    });

    preview_sprites.push_back(rm.getBitmap("yellowbird-midflap"));
    preview_sprites.push_back(rm.getBitmap("nerd_1"));
    preview_sprites.push_back(rm.getBitmap("barbielaco_1"));
    preview_sprites.push_back(rm.getBitmap("pig_1"));

    positionsX = {30, 94, 158, 222};
}

void CharacterSelectionScene::processEvent(const ALLEGRO_EVENT &event)
{
    if (event.type != ALLEGRO_EVENT_KEY_DOWN)
        return;

    if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
    {
        selectedIndex = (selectedIndex + 1) % themes.size();
    }
    else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
    {
        selectedIndex = (selectedIndex + themes.size() - 1) % themes.size();
    }
    else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
    {
        std::cout << "Tema selecionado: " << themes[selectedIndex].name << std::endl;
        // Ao criar a GameScene, o tema selecionado será passado e a música correta será usada
        sceneManager->setCurrentScene(std::make_unique<GameScene>(sceneManager, themes[selectedIndex]));
    }
}

void CharacterSelectionScene::update(float deltaTime)
{
}

void CharacterSelectionScene::draw() const
{
    al_draw_bitmap(themes[selectedIndex].background, 0, 0, 0);

    // Texto do tema
    al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, 80, ALLEGRO_ALIGN_CENTER, "Escolha seu Tema");

    // Sprites dos pássaros
    for (size_t i = 0; i < preview_sprites.size(); ++i)
    {
        float yPos = (themes[i].name == "Barbie") ? 122.0f : 132.0f;
        al_draw_bitmap(preview_sprites[i], positionsX[i], yPos, 0);
    }

    // Seleção retangular
    float x = positionsX[selectedIndex];
    float w = al_get_bitmap_width(preview_sprites[selectedIndex]);
    float h = al_get_bitmap_height(preview_sprites[selectedIndex]);
    float y = (themes[selectedIndex].name == "Barbie") ? 120.0f : 130.0f;
    al_draw_rectangle(x - 2, y - 2, x + w + 2, y + h + 2, selection_color, 2.0f);

    // Nome do Tema
    al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, 220, ALLEGRO_ALIGN_CENTER, themes[selectedIndex].name.c_str());
}