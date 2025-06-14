#pragma once

#include <memory>
#include <Scene.hpp>

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void set_current_scene(std::unique_ptr<Scene> newScene);
    void processEvent(const ALLEGRO_EVENT& event);
    void update(float deltaTime);
    void draw();

    bool is_running() const { return running; }
    void shutdown() { running = false; }

private:
    void transition_scene();

    std::unique_ptr<Scene> current_scene;
    std::unique_ptr<Scene> next_scene;
    bool running;
};