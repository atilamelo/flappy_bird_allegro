/**
 * @file PipePool.cpp
 * @brief Implementação da classe PipePool, responsável por gerenciar um pool de PipePairs.
 */

#include "actors/PipePool.hpp"
#include <iostream>

/**
 * @brief Inicializa o pool com a quantidade especificada de PipePairs.
 * @param initialSize Número inicial de PipePairs no pool.
 */
PipePool::PipePool(size_t initialSize)
{
    for (size_t i = 0; i < initialSize; ++i)
    {
        pool.push_back(std::make_unique<PipePair>());
    }
}

/**
 * @brief Obtém um PipePair inativo para reutilização.
 * Se não houver PipePair inativo, cria um novo e adiciona ao pool.
 * @return Ponteiro para PipePair disponível.
 */
PipePair* PipePool::getPipe()
{
    for (auto& pipePair : pool)
    {
        if (!pipePair->isActive())
        {
            return pipePair.get();
        }
    }

    // Se não houver canos inativos, cria um novo e adiciona ao pool
    std::cout << "Criando novo PipePair, pois não há canos inativos. Canos no pool: " << pool.size() << std::endl;
    pool.push_back(std::make_unique<PipePair>());
    return pool.back().get();
}

/**
 * @brief Atualiza todos os PipePairs do pool.
 * @param deltaTime Tempo decorrido desde a última atualização.
 */
void PipePool::update(float deltaTime)
{
    for (auto& pipePair : pool)
    {
        pipePair->update(deltaTime);
    }
}

/**
 * @brief Desenha todos os PipePairs do pool.
 */
void PipePool::draw() const
{
    for (auto& pipePair : pool)
    {
        pipePair->draw();
    }
}

/**
 * @brief Reseta todos os PipePairs do pool para o estdo inicial.
 */
void PipePool::reset() {
    for (auto& pipePair : pool) {
        pipePair->reset();
    }
}