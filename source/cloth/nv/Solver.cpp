#include "uniphysics/cloth/nv/Solver.h"
#include "uniphysics/cloth/nv/Cloth.h"
#include "uniphysics/cloth/nv/Factory.h"

#include <NvCloth/Factory.h>
#include <NvCloth/Solver.h>

namespace up
{
namespace cloth
{
namespace nv
{

Solver::Solver(Factory& factory)
{
    m_impl = factory.GetImpl()->createSolver();
    assert(m_impl);
}

Solver::~Solver()
{
    NV_CLOTH_DELETE(m_impl);
}

void Solver::AddCloth(const std::shared_ptr<cloth::Cloth>& cloth)
{
    m_impl->addCloth(std::static_pointer_cast<nv::Cloth>(cloth)->GetImpl());
}

void Solver::RemoveCloth(const std::shared_ptr<cloth::Cloth>& cloth)
{
    m_impl->removeCloth(std::static_pointer_cast<nv::Cloth>(cloth)->GetImpl());
}

bool Solver::BeginSimulation(float dt)
{
    return m_impl->beginSimulation(dt);
}

void Solver::SimulateChunk(int idx)
{
    m_impl->simulateChunk(idx);
}

void Solver::EndSimulation()
{
    m_impl->endSimulation();
}

int Solver::GetSimulationChunkCount() const
{
    return m_impl->getSimulationChunkCount();
}

}
}
}