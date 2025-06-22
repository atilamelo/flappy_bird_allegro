#pragma once

#include <memory>
#include <vector>
#include "actors/PipePair.hpp"

class PipePool : public IDrawable, public IUpdatable
{
private:
    std::vector<std::unique_ptr<PipePair>> pool;

public:
    PipePool(size_t initialSize = 10);

    PipePair* getPipe();
    std::vector<PipePair*>& getPipes() {
        return reinterpret_cast<std::vector<PipePair*>&>(pool);
     }

    void update(float deltaTime) override;
    void draw() const override;
    void reset();
};