#pragma once

#include <allegro5/allegro.h>
#include "SceneManager.hpp"

// Interface for a game scene (screen)
class Scene {
protected:
    SceneManager* sceneManager;

public:

    Scene(SceneManager* sceneManager) : sceneManager(sceneManager) {}

    virtual ~Scene() {}

    virtual void loadAssets() = 0;

    virtual void update(float deltaTime) = 0;

    virtual void draw() = 0;

    virtual void processEvent(const ALLEGRO_EVENT& event) = 0;
};