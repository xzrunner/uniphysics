#include "uniphysics/rigid/box2d/Joint.h"
#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/config.h"

#include <box2d/b2_mouse_joint.h>

#include <stdexcept>

namespace up
{
namespace rigid
{
namespace box2d
{

Joint::Joint(JointType type, const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b)
	: m_type(type)
	, m_body_a(body_a)
	, m_body_b(body_b)
{
}

void Joint::SetImpl(b2Joint* joint)
{
	assert(!m_impl);
	if (m_impl) {
		throw std::runtime_error("Joint not empty.");
	}

	m_impl = joint;
}

PrismaticJoint::PrismaticJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
	                           const sm::vec2& anchor, const sm::vec2& axis)
	: Joint(JointType::Prismatic, body_a, body_b)
	, m_anchor(anchor)
	, m_axis(axis)
{
}

MouseJoint::MouseJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
	                   const sm::vec2& target, float max_force)
	: Joint(JointType::Mouse, body_a, body_b)
	, m_target(target / SCALE_FACTOR)
	, m_max_force(max_force)
{
}

void MouseJoint::SetTarget(const sm::vec2& target)
{
	auto impl = GetImpl();
	if (!impl) {
		return;
	}
	static_cast<b2MouseJoint*>(impl)->SetTarget({ target.x / SCALE_FACTOR, target.y / SCALE_FACTOR });
}

}
}
}