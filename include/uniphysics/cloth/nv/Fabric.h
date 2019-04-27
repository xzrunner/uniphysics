#pragma once

#include "uniphysics/cloth/Fabric.h"

#include <NvCloth/Allocator.h>

#include <vector>

namespace nv
{
namespace cloth
{
    class Fabric;
    class ClothMeshDesc;
}
}

namespace physx { class PxVec3; }

namespace up
{
namespace cloth
{
namespace nv
{

class Factory;

class Fabric final : public cloth::Fabric
{
public:
    Fabric(Factory& factory, const ::nv::cloth::ClothMeshDesc& desc, const physx::PxVec3& gravity,
        ::nv::cloth::Vector<int32_t>::Type* phase_types = nullptr, bool use_geodesic_tether = true);

    virtual uint32_t GetNumPhases() const override;

    auto GetImpl() { return m_impl; }

private:
    ::nv::cloth::Fabric* m_impl = nullptr;

}; // Fabric

}
}
}