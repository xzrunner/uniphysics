#pragma once

#include <SM_Vector.h>

#include <memory>

class b2Joint;

namespace up
{
namespace rigid
{
namespace box2d
{

class Body;

enum class JointType
{
	Unknown,
	Revolute,
	Prismatic,
	Distance,
	Pulley,
	Mouse,
	Gear,
	Wheel,
	Weld,
	Friction,
	Rope,
	Motor
};

class Joint
{
public:
	Joint(JointType type, const std::shared_ptr<Body>& body_a,
		const std::shared_ptr<Body>& body_b);
	virtual ~Joint() {}

	auto GetType() const { return m_type; }

	auto GetBodyA() const { return m_body_a; }
	auto GetBodyB() const { return m_body_b; }

	void SetImpl(b2Joint* joint);
	auto GetImpl() const { return m_impl; }

private:
	JointType m_type = JointType::Unknown;

	std::shared_ptr<Body> m_body_a = nullptr;
	std::shared_ptr<Body> m_body_b = nullptr;

	b2Joint* m_impl = nullptr;

}; // Joint

class PrismaticJoint : public Joint
{
public:
	PrismaticJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
		const sm::vec2& anchor, const sm::vec2& axis);

	auto& GetAnchor() const { return m_anchor; }
	auto& GetAxis() const { return m_axis; }

private:
	sm::vec2 m_anchor;
	sm::vec2 m_axis;

}; // PrismaticJoint

}
}
}