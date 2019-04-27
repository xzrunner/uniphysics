#pragma once

#include "uniphysics/cloth/Cloth.h"

#include <NvCloth/Range.h>

namespace nv { namespace cloth { class Cloth; } }
namespace physx { class PxVec4; }

namespace up
{
namespace cloth
{
namespace nv
{

class Factory;
class Fabric;

class Cloth final : public cloth::Cloth
{
public:
    Cloth(Factory& factory, Fabric& fabric, ::nv::cloth::Range<const physx::PxVec4> particles);
    virtual ~Cloth();

    virtual void SetGravity(const sm::vec3& gravity) override;
    virtual sm::vec3 GetGravity() const override;

    auto GetImpl() { return m_impl; }

private:
    ::nv::cloth::Cloth* m_impl = nullptr;

}; // Cloth

}
}
}