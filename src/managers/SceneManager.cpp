/**
 * @file SceneManager.cpp
 * @brief Implementação do gerenciador de cenas com transições.
 */
#include "managers/SceneManager.hpp"
#include "core/Scene.hpp"

SceneManager::SceneManager() {
    current_scene = nullptr;
    next_scene = nullptr;
    running = true;
    state = TransitionState::RUNNING;
    event_queue = nullptr;
    transitionEffect = std::make_unique<TransitionEffect>(al_map_rgb(0, 0, 0), 0.5f);
}

SceneManager::~SceneManager() {}

void SceneManager::setCurrentScene(std::unique_ptr<Scene> newScene) {
    // Só inicia uma nova transição se não houver outra em andamento.
    if (state == TransitionState::RUNNING) {
        next_scene = std::move(newScene);
        state = TransitionState::FADE_OUT; // Inicia o processo de fade-out
        transitionEffect->fadeOut();
    }
}

void SceneManager::processEvent(const ALLEGRO_EVENT& event) {
    if (current_scene) {
        current_scene->processEvent(event);
    }
}

void SceneManager::update(float deltaTime) {
    // A lógica de atualização agora é uma máquina de estados.
    switch (state) {
        case TransitionState::RUNNING:
            // Estado normal: apenas atualiza a cena atual.
            if (current_scene) {
                current_scene->update(deltaTime);
            }
            break;

        case TransitionState::FADE_OUT:
            // Escurecendo a tela: atualiza a cena atual E o efeito.
            if (current_scene) {
                current_scene->update(deltaTime);
            }
            transitionEffect->update(deltaTime);

            // Quando o fade-out terminar...
            if (transitionEffect->isFinished()) {
                current_scene = std::move(next_scene); // ...troca a cena...
                state = TransitionState::FADE_IN;     // ...muda para o estado de fade-in...
                transitionEffect->fadeIn();           // ...e inicia o efeito de fade-in.
            }
            break;

        case TransitionState::FADE_IN:
            // Clareando a tela: atualiza a NOVA cena e o efeito.
            if (current_scene) {
                current_scene->update(deltaTime);
            }
            transitionEffect->update(deltaTime);

            // Quando o fade-in terminar, volta ao estado normal.
            if (transitionEffect->isFinished()) {
                state = TransitionState::RUNNING;
            }
            break;
    }
}

void SceneManager::draw() {
    // 1. Sempre desenha a cena atual, seja a antiga ou a nova.
    if (current_scene) {
        current_scene->draw();
    }

    // 2. Sempre desenha o efeito de transição por cima.
    // Se ele estiver inativo (alfa=0), nada será desenhado.
    transitionEffect->draw();
}