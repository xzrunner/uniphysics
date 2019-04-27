#include "uniphysics/cloth/nv/Fabric.h"
#include "uniphysics/cloth/nv/Factory.h"

#include <NvClothExt/ClothFabricCooker.h>

#include <assert.h>

namespace up
{
namespace cloth
{
namespace nv
{

Fabric::Fabric(Factory& factory, const ::nv::cloth::ClothMeshDesc& desc, const physx::PxVec3& gravity,
               ::nv::cloth::Vector<int32_t>::Type* phase_types, bool use_geodesic_tether)
{
    m_impl = NvClothCookFabricFromMesh(factory.GetImpl(), desc, gravity, phase_types, use_geodesic_tether);
}

uint32_t Fabric::GetNumPhases() const
{
    assert(m_impl);
    return m_impl->getNumPhases();
}

}
}
}