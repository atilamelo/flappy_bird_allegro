#include "actors/PipePool.hpp"
#include <iostream>

PipePool::PipePool(size_t initialSize)
{
    for (size_t i = 0; i < initialSize; ++i)
    {
        pool.push_back(std::make_unique<PipePair>());
    }
}

PipePair* PipePool::getPipe()
{
    for (auto& pipePair : pool)
    {
        if (!pipePair->isActive())
        {
            return pipePair.get();
        }
    }

    // Se não houver canos inativos, cria um novo
    std::cout << "Criando novo PipePair, pois não há canos inativos. Canos no pool: " << pool.size() << std::endl;
    pool.push_back(std::make_unique<PipePair>());
    return pool.back().get();
}

void PipePool::update(float deltaTime)
{
    for (auto& pipePair : pool)
    {
        pipePair->update(deltaTime);
    }
}

void PipePool::draw()
{
    for (auto& pipePair : pool)
    {
        pipePair->draw();
    }
}

void PipePool::reset() {
    for (auto& pipePair : pool) {
        pipePair->reset();
    }
}