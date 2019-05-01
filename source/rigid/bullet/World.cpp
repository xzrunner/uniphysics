#include "uniphysics/rigid/bullet/World.h"
#include "uniphysics/rigid/bullet/Body.h"

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

namespace up
{
namespace rigid
{
namespace bullet
{

World::World()
{
    Init();
}

void World::AddBody(const std::shared_ptr<rigid::Body>& body)
{
    m_world->addRigidBody(std::static_pointer_cast<Body>(body)->GetImpl().get());
}

void World::StepSimulation(float dt)
{
    m_world->stepSimulation(dt);
}

void World::Init()
{
    ///collision configuration contains default setup for memory, collision setup
    m_collision_cfg = std::make_unique<btDefaultCollisionConfiguration>();
    //m_collision_cfg->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collision_cfg.get());

    m_broadphase = std::make_unique<btDbvtBroadphase>();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

    m_world = std::make_shared<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collision_cfg.get());
    m_world->setGravity(btVector3(0, -10, 0));
}

}
}
}