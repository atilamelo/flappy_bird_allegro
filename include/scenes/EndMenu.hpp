#pragma once

#include "core/Scene.hpp"
#include "Constants.hpp"
#include "widgetz/widgetz.h"

class EndMenu : public Scene {
    private:
        WZ_WIDGET* gui;
        WZ_DEF_THEME theme;
        WZ_SKIN_THEME skin_theme;
        WZ_WIDGET* wgt;
        ALLEGRO_FONT *font;
        float size = 1.0;
        ALLEGRO_EVENT_QUEUE *guiQueue;

    public:
        EndMenu(SceneManager *sceneManager);
        ~EndMenu();
        void loadAssets();
        void processEvent(const ALLEGRO_EVENT& event) override;
        void update(float deltaTime) override;
        void gameOver();
        void draw() const override;
        void getQueueFrom(ALLEGRO_EVENT_QUEUE& queue);
};