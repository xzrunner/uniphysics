#include "uniphysics/rigid/box2d/World.h"
#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/DebugDraw.h"
#include "uniphysics/rigid/box2d/Joint.h"
#include "uniphysics/rigid/box2d/config.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_revolute_joint.h>
#include <box2d/b2_prismatic_joint.h>
#include <box2d/b2_distance_joint.h>
#include <box2d/b2_mouse_joint.h>
#include <box2d/b2_wheel_joint.h>

namespace
{

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point) 
		: m_point(point) {}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

public:
	b2Vec2 m_point;
	b2Fixture* m_fixture = NULL;

}; // QueryCallback

}

namespace up
{
namespace rigid
{
namespace box2d
{

static World::Callback CB;

void World::RegisterCallback(const World::Callback& cb)
{
	CB = cb;
}

class ContactListener : public b2ContactListener
{
public:
	ContactListener(const World& world) 
		: m_world(world)
	{
	}

	virtual void BeginContact(b2Contact* contact) override
	{
		auto a = FindBody(contact->GetFixtureA());
		auto b = FindBody(contact->GetFixtureB());
		if (a && b && CB.begin_contact) {
			CB.begin_contact(a, b);
		}
	}

	virtual void EndContact(b2Contact* contact) override
	{
		auto a = FindBody(contact->GetFixtureA());
		auto b = FindBody(contact->GetFixtureB());
		if (a && b && CB.end_contact) {
			CB.end_contact(a, b);
		}
	}

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
		auto a = FindBody(contact->GetFixtureA());
		auto b = FindBody(contact->GetFixtureB());
		if (a && b && CB.pre_solve) {
			CB.pre_solve(a, b);
		}
	}

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{
		auto a = FindBody(contact->GetFixtureA());
		auto b = FindBody(contact->GetFixtureB());
		if (a && b && CB.post_solve) {
			CB.post_solve(a, b);
		}
	}

private:
	std::shared_ptr<Body> FindBody(const b2Fixture* fixture) const
	{
		auto b = fixture->GetBody();
		for (auto& body : m_world.m_bodies) {
			if (body->GetImpl() == b) {
				return body;
			}
		}
		return nullptr;
	}

private:
	const World& m_world;

}; // ContactListener

World::World()
{
	b2Vec2 b2gravity;
	b2gravity.Set(0.0f, -10.0f);
	m_impl = new b2World(b2gravity);

	m_contact_lsn = std::make_unique<ContactListener>(*this);
	m_impl->SetContactListener(m_contact_lsn.get());
}

World::~World()
{
	assert(!m_impl->IsLocked());

	for (auto& joint : m_joints) {
		m_impl->DestroyJoint(joint->GetImpl());
	}
	for (auto& body : m_bodies) {
		m_impl->DestroyBody(body->GetImpl());
	}

	delete m_impl;
}

void World::AddBody(const std::shared_ptr<rigid::Body>& body)
{
	auto b2_body = std::static_pointer_cast<box2d::Body>(body);

	auto& type = b2_body->GetType();

	b2BodyDef bd;
	if (type == "static") {
		bd.type = b2_staticBody;
	} else if (type == "kinematic") {
		bd.type = b2_kinematicBody;
	} else if (type == "dynamic") {
		bd.type = b2_dynamicBody;
	}
	auto _body = m_impl->CreateBody(&bd);
	b2_body->SetImpl(_body);

	m_bodies.push_back(b2_body);
}

void World::RemoveBody(const std::shared_ptr<rigid::Body>& body)
{
	if (m_impl->IsLocked()) {
		m_destroy_bodies.push_back(body);
	} else {
		RemoveBodyForce(body);
	}
}

void World::StepSimulation(float dt)
{
	PreSimulation();

	m_impl->Step(dt, m_velocity_iter, m_position_iter);
}

World::Picked World::PickBody(const sm::vec3& ray_from, const sm::vec3& ray_to) const
{
	return World::Picked();
}

bool World::MovePickedBody(const Picked& picked, const sm::vec3& ray_from, const sm::vec3& ray_to)
{
	return false;
}

void World::RemoveConstraint(const std::shared_ptr<rigid::Constraint>& cons)
{

}

void World::AddJoint(const std::shared_ptr<Joint>& joint)
{
	auto body_a = joint->GetBodyA()->GetImpl();
	auto body_b = joint->GetBodyB()->GetImpl();
	if (!body_a || !body_b) {
		return;
	}

	switch (joint->GetType())
	{
	case JointType::Revolute:
	{
		auto revolute = std::static_pointer_cast<RevoluteJoint>(joint);

		b2RevoluteJointDef jd;

		auto body_a = joint->GetBodyA()->GetImpl();
		auto body_b = joint->GetBodyB()->GetImpl();
		auto& anchor = revolute->GetAnchor();
		jd.Initialize(body_a, body_b, { anchor.x, anchor.y });

		revolute->GetAngleLimit(jd.enableLimit, jd.lowerAngle, jd.upperAngle);
		revolute->GetMotor(jd.enableMotor, jd.maxMotorTorque, jd.motorSpeed);

		joint->SetImpl(m_impl->CreateJoint(&jd));

		m_joints.push_back(joint);
	}
		break;
	case JointType::Prismatic:
	{
		auto prismatic = std::static_pointer_cast<PrismaticJoint>(joint);

		b2PrismaticJointDef jd;

		auto body_a = joint->GetBodyA()->GetImpl();
		auto body_b = joint->GetBodyB()->GetImpl();
		auto& anchor = prismatic->GetAnchor();
		auto& axis = prismatic->GetAxis();
		jd.Initialize(body_a, body_b, { anchor.x, anchor.y }, { axis.x, axis.y });

		prismatic->GetTranslateLimit(jd.enableLimit, jd.lowerTranslation, jd.upperTranslation);
		prismatic->GetMotor(jd.enableMotor, jd.maxMotorForce, jd.motorSpeed);

		joint->SetImpl(m_impl->CreateJoint(&jd));

		m_joints.push_back(joint);
	}
		break;
	case JointType::Distance:
	{
		auto distance = std::static_pointer_cast<DistanceJoint>(joint);

		b2DistanceJointDef jd;

		auto body_a = joint->GetBodyA()->GetImpl();
		auto body_b = joint->GetBodyB()->GetImpl();
		auto& anchor_a = distance->GetAnchorA();
		auto& anchor_b = distance->GetAnchorB();
		jd.Initialize(body_a, body_b, { anchor_a.x, anchor_a.y }, { anchor_b.x, anchor_b.y });

		distance->GetLength(jd.minLength, jd.maxLength);

		float hertz = 1.0f;
		float damping_ratio = 0.7f;
		b2LinearStiffness(jd.stiffness, jd.damping, hertz, damping_ratio, jd.bodyA, jd.bodyB);

		joint->SetImpl(m_impl->CreateJoint(&jd));

		m_joints.push_back(joint);
	}
		break;
	case JointType::Mouse:
	{
		auto mouse = std::static_pointer_cast<MouseJoint>(joint);

		b2MouseJointDef jd;

		jd.bodyA = joint->GetBodyA()->GetImpl();
		jd.bodyB = joint->GetBodyB()->GetImpl();
		jd.target = b2Vec2(mouse->GetTarget().x, mouse->GetTarget().y);
		jd.maxForce = mouse->GetMaxForce();

		float frequencyHz = 5.0f;
		float dampingRatio = 0.7f;
		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

		joint->SetImpl(m_impl->CreateJoint(&jd));

		m_joints.push_back(joint);
	}
		break;
	case JointType::Wheel:
	{
		auto wheel = std::static_pointer_cast<WheelJoint>(joint);

		b2WheelJointDef jd;

		auto body_a = joint->GetBodyA()->GetImpl();
		auto body_b = joint->GetBodyB()->GetImpl();
		auto& anchor = wheel->GetAnchor();
		auto& axis = wheel->GetAxis();
		jd.Initialize(body_a, body_b, { anchor.x, anchor.y }, { axis.x, axis.y });

		wheel->GetTranslateLimit(jd.enableLimit, jd.lowerTranslation, jd.upperTranslation);
		wheel->GetMotor(jd.enableMotor, jd.maxMotorTorque, jd.motorSpeed);
		wheel->GetSuspension(jd.stiffness, jd.damping);

		joint->SetImpl(m_impl->CreateJoint(&jd));

		m_joints.push_back(joint);
	}
		break;
	}
}

void World::RemoveJoint(const std::shared_ptr<Joint>& joint)
{
	if (m_impl->IsLocked()) {
		m_destroy_joints.push_back(joint);
	} else {
		RemoveJointForce(joint);
	}
}

void World::SetDebugDraw(rigid::DebugDraw& draw)
{
	m_impl->SetDebugDraw(&static_cast<box2d::DebugDraw&>(draw));
}

void World::DebugDraw() const
{
	m_impl->DebugDraw();
}

std::shared_ptr<Body> World::QueryByPos(const sm::vec2& pos) const
{
	const b2Vec2 b2Pos(pos.x / SCALE_FACTOR, pos.y / SCALE_FACTOR);

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = b2Pos - d;
	aabb.upperBound = b2Pos + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(b2Pos);
	m_impl->QueryAABB(&callback, aabb);

	if (!callback.m_fixture) {
		return nullptr;
	}

	auto b = callback.m_fixture->GetBody();
	for (auto body : m_bodies) {
		if (body->GetImpl() == b) {
			return body;
		}
	}

	return nullptr;
}

void World::PreSimulation()
{
	if (m_impl->IsLocked()) {
		return;
	}

	for (auto& body : m_destroy_bodies) {
		RemoveBodyForce(body);
	}
	m_destroy_bodies.clear();

	for (auto& joint : m_destroy_joints) {
		RemoveJointForce(joint);
	}
	m_destroy_bodies.clear();
}

void World::RemoveBodyForce(const std::shared_ptr<rigid::Body>& body)
{
	for (auto itr = m_bodies.begin(); itr != m_bodies.end(); )
	{
		if (*itr == body)
		{
			auto b2_body = std::static_pointer_cast<box2d::Body>(body);
			b2_body->DeleteImpl(m_impl);
			itr = m_bodies.erase(itr);
			return;
		}
		else
		{
			++itr;
		}
	}
}

void World::RemoveJointForce(const std::shared_ptr<Joint>& joint)
{
	for (auto itr = m_joints.begin(); itr != m_joints.end(); )
	{
		if (*itr == joint)
		{
			joint->DeleteImpl(m_impl);
			itr = m_joints.erase(itr);
			return;
		}
		else
		{
			++itr;
		}
	}
}

}
}
}