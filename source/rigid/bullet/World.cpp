#include "uniphysics/rigid/bullet/World.h"
#include "uniphysics/rigid/bullet/Body.h"
#include "uniphysics/rigid/bullet/Constraint.h"
#include "uniphysics/rigid/bullet/Utility.h"

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>

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
    auto bt_body = std::static_pointer_cast<Body>(body)->GetImpl().get();
    m_impl->addRigidBody(bt_body);
    m_map_bt.insert({ bt_body, body });
}

void World::RemoveBody(const std::shared_ptr<rigid::Body>& body)
{
}

void World::StepSimulation(float dt)
{
    m_impl->stepSimulation(dt);
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

    m_impl = std::make_shared<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collision_cfg.get());
    m_impl->setGravity(btVector3(0, -10, 0));
}

World::Picked World::PickBody(const sm::vec3& ray_from, const sm::vec3& ray_to) const
{
    if (!m_impl) {
        return World::Picked();
    }

    World::Picked ret;

    btVector3 bt_r_from = vec3_sm2bt(ray_from);
    btVector3 bt_r_to   = vec3_sm2bt(ray_to);

    btCollisionWorld::ClosestRayResultCallback rayCallback(bt_r_from, bt_r_to);

    rayCallback.m_flags |= btTriangleRaycastCallback::kF_UseGjkConvexCastRaytest;
    m_impl->rayTest(bt_r_from, bt_r_to, rayCallback);
    if (rayCallback.hasHit())
    {
			btVector3 pick_pos = rayCallback.m_hitPointWorld;
			btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
			if (body)
			{
				//other exclusions?
				if (!(body->isStaticObject() || body->isKinematicObject()))
				{
                    auto itr = m_map_bt.find(body);
                    assert(itr != m_map_bt.end());
                    ret.body = itr->second;
                    ret.saved_state = body->getActivationState();
                    body->setActivationState(DISABLE_DEACTIVATION);
					//printf("pick_pos=%f,%f,%f\n",pick_pos.getX(),pick_pos.getY(),pick_pos.getZ());
					btVector3 local_pivot = body->getCenterOfMassTransform().inverse() * pick_pos;
					auto p2p = std::make_unique<btPoint2PointConstraint>(*body, local_pivot);
					m_impl->addConstraint(p2p.get(), true);

					btScalar mouse_pick_clamping = 30.f;
					p2p->m_setting.m_impulseClamp = mouse_pick_clamping;
					//very weak constraint for picking
					p2p->m_setting.m_tau = 0.001f;
                    ret.cons = std::make_shared<Constraint>(std::move(p2p));
				}
			}

			//					pickObject(pick_pos, rayCallback.m_collisionObject);
			ret.old_picking_pos = ray_to;
			ret.hit_pos = vec3_bt2sm(pick_pos);
            ret.old_picking_dist = (vec3_bt2sm(pick_pos) - ray_from).Length();
			//					printf("hit !\n");
			//add p2p
    }

    return ret;
}

bool World::MovePickedBody(const Picked& picked, const sm::vec3& ray_from, const sm::vec3& ray_to)
{
    auto cons = std::static_pointer_cast<Constraint>(picked.cons);
	btPoint2PointConstraint* pick_con = static_cast<btPoint2PointConstraint*>(cons->GetImpl().get());
    if (!pick_con) {
        return false;
    }

	//keep it at the same picking distance

	auto dir = ray_to - ray_from;
	dir.Normalize();
	dir *= picked.old_picking_dist;

	auto new_pivot_b = ray_from + dir;
	pick_con->setPivotB(vec3_sm2bt(new_pivot_b));

	return true;
}

void World::RemoveConstraint(const std::shared_ptr<rigid::Constraint>& cons)
{
    m_impl->removeConstraint(std::static_pointer_cast<Constraint>(cons)->GetImpl().get());
}

}
}
}