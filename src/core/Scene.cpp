/**
 * @file Scene.cpp
 * @brief Implementação dos métodos da classe base Scene.
 */
#include "core/Scene.hpp"

// Definição do construtor
Scene::Scene(SceneManager* sceneManager)
    : sceneManager(sceneManager)
{}

// Definição do destrutor virtual.
Scene::~Scene()
{}