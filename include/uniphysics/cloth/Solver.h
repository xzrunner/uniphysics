#pragma once

#include "uniphysics/noncopyable.h"

#include <memory>

namespace up
{
namespace cloth
{

class Cloth;

class Solver : noncopyable
{
public:
    virtual ~Solver() {}

    virtual void AddCloth(const std::shared_ptr<Cloth>& cloth) = 0;
    virtual void RemoveCloth(const std::shared_ptr<Cloth>& cloth) = 0;

    virtual bool BeginSimulation(float dt) = 0;
    virtual void SimulateChunk(int idx) = 0;
    virtual void EndSimulation() = 0;
    virtual int  GetSimulationChunkCount() const = 0;

}; // Solver

}
}