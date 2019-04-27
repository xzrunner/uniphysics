#pragma once

#include "uniphysics/cloth/Solver.h"

namespace nv { namespace cloth { class Solver; } }

namespace up
{
namespace cloth
{
namespace nv
{

class Factory;

class Solver final : public cloth::Solver
{
public:
    Solver(Factory& factory);
    virtual ~Solver();

    virtual void AddCloth(const std::shared_ptr<cloth::Cloth>& cloth) override;
    virtual void RemoveCloth(const std::shared_ptr<cloth::Cloth>& cloth) override;

    virtual bool BeginSimulation(float dt) override;
    virtual void SimulateChunk(int idx) override;
    virtual void EndSimulation() override;
    virtual int  GetSimulationChunkCount() const override;

private:
    ::nv::cloth::Solver* m_impl = nullptr;

}; // Solver

}
}
}