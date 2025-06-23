#pragma once

#include "core/Scene.hpp"
#include <memory>

class SceneManager {
public:
    SceneManager();
    SceneManager(ALLEGRO_EVENT_QUEUE* queue);
    ~SceneManager();

    void set_current_scene(std::unique_ptr<Scene> newScene);
    void setEventQueue(ALLEGRO_EVENT_QUEUE* queue) { event_queue = queue; };
    void processEvent(const ALLEGRO_EVENT& event);
    void update(float deltaTime);
    void draw();

    bool is_running() const { return running; }
    void shutdown() { running = false; }

    ALLEGRO_EVENT_QUEUE* get_event_queue() const { return event_queue; }

private:
    void transition_scene();

    std::unique_ptr<Scene> current_scene;
    std::unique_ptr<Scene> next_scene;
    bool running;
    ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
};