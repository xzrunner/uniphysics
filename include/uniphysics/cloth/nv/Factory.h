#pragma once

#include "uniphysics/cloth/Factory.h"

#include <NvCloth/Factory.h>

//#ifndef NV_CLOTH_FACTORY_DX11
//#define NV_CLOTH_FACTORY_DX11
//#endif // NV_CLOTH_FACTORY_DX11

namespace nv { namespace cloth { class Factory; } }

namespace up
{
namespace cloth
{
namespace nv
{

class Factory final : public cloth::Factory
{
public:
    Factory(::nv::cloth::Platform platform);
    virtual ~Factory();

    virtual std::shared_ptr<cloth::Solver> CreateSolver() override;

    virtual std::shared_ptr<cloth::Fabric>
        CreateFabric(const ClothMeshData& mesh_data, const sm::vec3& gravity) override;

    virtual std::unique_ptr<cloth::Cloth>
        CreateCloth(cloth::Fabric& fabric, const std::vector<sm::vec4>& particles) override;

    auto GetImpl() { return m_impl; }

private:
    ::nv::cloth::Factory* m_impl = nullptr;

#ifdef NV_CLOTH_FACTORY_DX11
    ::nv::cloth::DxContextManagerCallback* mGraphicsContextManager = nullptr;
#endif // NV_CLOTH_FACTORY_DX11

}; // Factory

}
}
}