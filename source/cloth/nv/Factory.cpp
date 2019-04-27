#include "uniphysics/cloth/nv/Factory.h"
#include "uniphysics/cloth/nv/Solver.h"
#include "uniphysics/cloth/nv/Fabric.h"
#include "uniphysics/cloth/nv/Cloth.h"
#include "uniphysics/cloth/nv/NvCloth.h"
#include "uniphysics/cloth/ClothMeshData.h"

#include <NvCloth/DxContextManagerCallback.h>
#include <NvClothExt/ClothMeshDesc.h>

#include <mutex>

#include <d3d11.h>

#include <assert.h>

namespace
{

class DxContextManagerCallbackImpl : public ::nv::cloth::DxContextManagerCallback
{
public:
    DxContextManagerCallbackImpl(ID3D11Device* device, bool synchronizeResources = false)
        : m_device(device)
        , m_synchronize_resources(synchronizeResources)
    {
        m_device->AddRef();
        m_device->GetImmediateContext(&m_context);
    }

	~DxContextManagerCallbackImpl()
    {
	    m_context->Release();
    #if _DEBUG
	    ID3D11Debug* debugDevice;
	    m_device->QueryInterface(&debugDevice);
	    if(debugDevice)
	    {
		    debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		    debugDevice->Release();
	    }
    #endif
	    m_device->Release();
    }

    virtual void acquireContext() override {
        m_mutex.lock();
    }
    virtual void releaseContext() override {
        m_mutex.unlock();
    }
    virtual ID3D11Device* getDevice() const override {
        return m_device;
    }
    virtual ID3D11DeviceContext* getContext() const override {
        return m_context;
    }
    virtual bool synchronizeResources() const override {
        return m_synchronize_resources;
    }

private:
	std::recursive_mutex m_mutex;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	bool m_synchronize_resources;
#ifdef _DEBUG
	uint32_t m_lock_count_tls;
#endif

}; // DxContextManagerCallbackImpl

}

namespace up
{
namespace cloth
{
namespace nv
{

Factory::Factory(::nv::cloth::Platform platform)
    : m_impl(nullptr)
{
    // init NvCloth callback
    NvCloth::Instance();

    switch (platform)
    {
    case ::nv::cloth::Platform::CPU:
        m_impl = NvClothCreateFactoryCPU();
        break;
    case ::nv::cloth::Platform::DX11:
        // todo
        break;
    case ::nv::cloth::Platform::CUDA:
        // todo
        break;
    }
    assert(m_impl);
}

Factory::~Factory()
{
    NvClothDestroyFactory(m_impl);
}

std::shared_ptr<cloth::Solver> Factory::CreateSolver()
{
    return std::make_shared<Solver>(*this);
}

std::shared_ptr<cloth::Fabric>
Factory::CreateFabric(const ClothMeshData& mesh_data, const sm::vec3& gravity)
{
    physx::PxVec3 g;
    g.x = gravity.x;
    g.y = gravity.y;
    g.z = gravity.z;
    return std::make_shared<Fabric>(*this, *mesh_data.GetClothMeshDesc(), g);
}

std::unique_ptr<cloth::Cloth>
Factory::CreateCloth(cloth::Fabric& fabric, const std::vector<sm::vec4>& particles)
{
    std::vector<physx::PxVec4> p_particles;
    p_particles.resize(particles.size());
    for (int i = 0, n = particles.size(); i < n; ++i) {
        p_particles[i].x = particles[i].x;
        p_particles[i].y = particles[i].y;
        p_particles[i].z = particles[i].z;
        p_particles[i].w = particles[i].w;
    }
    ::nv::cloth::Range<physx::PxVec4> range(&p_particles[0], &p_particles[0] + p_particles.size());
    return std::make_unique<Cloth>(*this, static_cast<cloth::nv::Fabric&>(fabric), range);
}

}
}
}