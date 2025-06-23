/**
 * @file SceneManager.hpp
 * @brief Definição do gerenciador de cenas com lógica de transição.
 */
#pragma once 

#include "core/Scene.hpp"
#include "actors/effects/TransitionEffect.hpp" // <-- Inclui o novo efeito
#include <memory>
#include <allegro5/allegro.h>

/**
 * @enum TransitionState
 * @brief Os estados do próprio gerenciador de cenas durante uma transição.
 */
enum class TransitionState {
    RUNNING,
    FADE_OUT,
    FADE_IN
};

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void setCurrentScene(std::unique_ptr<Scene> newScene);
    void setEventQueue(ALLEGRO_EVENT_QUEUE* queue) { event_queue = queue; }
    void processEvent(const ALLEGRO_EVENT& event);
    void update(float deltaTime);
    void draw();

    bool isRunning() const { return running; }
    void shutdown() { running = false; }
    ALLEGRO_EVENT_QUEUE* get_event_queue() { return event_queue; }


private:
    std::unique_ptr<Scene> current_scene;
    std::unique_ptr<Scene> next_scene;
    
    std::unique_ptr<TransitionEffect> transitionEffect;
    TransitionState state;
    
    bool running;
    ALLEGRO_EVENT_QUEUE* event_queue;
};