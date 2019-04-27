#include "uniphysics/cloth/nv/Cloth.h"
#include "uniphysics/cloth/nv/Factory.h"
#include "uniphysics/cloth/nv/Fabric.h"

#include <NvCloth/Cloth.h>

namespace up
{
namespace cloth
{
namespace nv
{

Cloth::Cloth(Factory& factory, Fabric& fabric, ::nv::cloth::Range<const physx::PxVec4> particles)
{
    m_impl = factory.GetImpl()->createCloth(particles, *fabric.GetImpl());
    assert(m_impl);
}

Cloth::~Cloth()
{
    NV_CLOTH_DELETE(m_impl);
}

void Cloth::SetGravity(const sm::vec3& g)
{
    m_impl->setGravity(physx::PxVec3(g.x, g.y, g.z));
}

sm::vec3 Cloth::GetGravity() const
{
    auto g = m_impl->getGravity();
    return sm::vec3(g.x, g.y, g.z);
}

}
}
}