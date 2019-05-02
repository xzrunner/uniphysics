#pragma once

#include "uniphysics/rigid/World.h"

#include <memory>
#include <map>

class btCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btRigidBody;

class btDiscreteDynamicsWorld;

namespace up
{
namespace rigid
{
namespace bullet
{

class World : public rigid::World
{
public:
    World();

    virtual void AddBody(const std::shared_ptr<rigid::Body>& body) override;

    virtual void StepSimulation(float dt) override;

    virtual Picked PickBody(const sm::vec3& ray_from, const sm::vec3& ray_to) const override;
    virtual bool MovePickedBody(const Picked& picked, const sm::vec3& ray_from, const sm::vec3& ray_to) override;

    virtual void RemoveConstraint(const std::shared_ptr<rigid::Constraint>& cons) override;

private:
    void Init();

private:
    std::unique_ptr<btCollisionConfiguration> m_collision_cfg = nullptr;
    std::unique_ptr<btDispatcher>             m_dispatcher    = nullptr;
    std::unique_ptr<btBroadphaseInterface>    m_broadphase    = nullptr;
    std::unique_ptr<btConstraintSolver>       m_solver        = nullptr;

    std::shared_ptr<btDiscreteDynamicsWorld> m_impl = nullptr;

    std::map<btRigidBody*, std::shared_ptr<rigid::Body>> m_map_bt;

}; // World

}
}
}