#include "SceneManager.hpp"
#include "Scene.hpp"

SceneManager::SceneManager() : current_scene(nullptr), next_scene(nullptr), running(true) {}

SceneManager::~SceneManager() {
}

void SceneManager::set_current_scene(std::unique_ptr<Scene> newScene) {
    next_scene = std::move(newScene);
}

void SceneManager::transition_scene() {
    if (!next_scene) return;

    current_scene = std::move(next_scene);
    current_scene->loadAssets();
}

void SceneManager::processEvent(const ALLEGRO_EVENT& event) {
    if (current_scene) {
        current_scene->processEvent(event);
    }
}

void SceneManager::update(float deltaTime) {
    transition_scene();

    if (current_scene) {
        current_scene->update(deltaTime);
    }
}

void SceneManager::draw() {
    if (current_scene) {
        current_scene->draw();
    }
}