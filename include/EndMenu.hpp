#pragma once

#include "Scene.hpp"
#include "Constants.hpp"
#include "widgetz/widgetz.h"

class EndMenu : public Scene {
    private:
        WZ_WIDGET* gui;
        WZ_DEF_THEME theme;
        WZ_SKIN_THEME skin_theme;
        ALLEGRO_FONT *font;
        float size = 1.0;
        ALLEGRO_EVENT_QUEUE *guiQueue;

    public:
        EndMenu(SceneManager *sceneManager);
        ~EndMenu();
        void loadAssets() override;
        void processEvent(const ALLEGRO_EVENT& event) override;
        void update(float deltaTime) override;
        void gameOver();
        void draw() override;
        void getQueueFrom(ALLEGRO_EVENT_QUEUE& queue);
};