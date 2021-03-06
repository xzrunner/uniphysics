#include "uniphysics/rigid/bullet/Body.h"
#include "uniphysics/rigid/bullet/Shape.h"
#include "uniphysics/rigid/bullet/Utility.h"

#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

namespace up
{
namespace rigid
{
namespace bullet
{

Body::Body(float mass, const sm::vec3& pos, const std::shared_ptr<rigid::Shape>& shape, const sm::vec4& color)
    : m_shape(shape)
{
    auto bt_shape = std::static_pointer_cast<Shape>(shape)->GetImpl().get();
	btAssert((!bt_shape || bt_shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool is_dynamic = (mass != 0.f);

	btVector3 local_inertia(0, 0, 0);
    if (is_dynamic) {
        bt_shape->calculateLocalInertia(mass, local_inertia);
    }

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(vec3_sm2bt(pos));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo c_info(mass, myMotionState, bt_shape, local_inertia);
    m_impl = std::make_unique<btRigidBody>(c_info);
	//m_impl->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
    m_impl = std::make_unique<btRigidBody>(mass, 0, bt_shape, local_inertia);
    m_impl->setWorldTransform(startTransform);
#endif  //

    m_impl->setUserIndex(-1);
}

sm::vec3 Body::GetPosition() const
{
    return vec3_bt2sm(m_impl->getWorldTransform().getOrigin());
}

void Body::ForceActivationState(int state)
{
    m_impl->forceActivationState(state);
}

void Body::Activate()
{
    m_impl->activate();
}

}
}
}