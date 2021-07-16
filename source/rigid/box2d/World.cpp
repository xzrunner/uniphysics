#include "uniphysics/rigid/box2d/World.h"
#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/DebugDraw.h"
#include "uniphysics/rigid/box2d/Joint.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_prismatic_joint.h>

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
		if (a && b) {
			CB.begin_contact(a, b);
		}
	}

	virtual void EndContact(b2Contact* contact) override
	{
	}

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{

	}

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{

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
		return;
	}

	for (auto itr = m_bodies.begin(); itr != m_bodies.end(); )
	{
		if (*itr == body) 
		{
			auto b2_body = std::static_pointer_cast<box2d::Body>(body);
			m_impl->DestroyBody(b2_body->GetImpl());
			itr = m_bodies.erase(itr);
			return;
		} 
		else 
		{
			++itr;
		}
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
	switch (joint->GetType())
	{
	case JointType::Prismatic:
	{
		auto prismatic = std::static_pointer_cast<PrismaticJoint>(joint);

		b2PrismaticJointDef pjd;

		auto body_a = joint->GetBodyA()->GetImpl();
		auto body_b = joint->GetBodyB()->GetImpl();
		auto& anchor = prismatic->GetAnchor();
		auto& axis = prismatic->GetAxis();
		pjd.Initialize(body_a, body_b, { anchor.x, anchor.y }, { axis.x, axis.y });

		pjd.motorSpeed = 0;
		pjd.maxMotorForce = 10000.0f;
		pjd.enableMotor = false;
		pjd.lowerTranslation = -10.0f;
		pjd.upperTranslation = 10.0f;
		pjd.enableLimit = true;

		joint->SetImpl(m_impl->CreateJoint(&pjd));

		m_joints.push_back(joint);
	}
		break;
	}
}

void World::RemoveJoint(const std::shared_ptr<Joint>& joint)
{
	if (m_impl->IsLocked()) {
		m_destroy_joints.push_back(joint);
		return;
	}

	for (auto itr = m_joints.begin(); itr != m_joints.end(); )
	{
		if (*itr == joint)
		{
			m_impl->DestroyJoint(joint->GetImpl());
			itr = m_joints.erase(itr);
			return;
		}
		else
		{
			++itr;
		}
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

void World::PreSimulation()
{
	for (auto& body : m_destroy_bodies)
	{
		for (auto itr = m_bodies.begin(); itr != m_bodies.end(); )
		{
			if (*itr == body)
			{
				auto b2_body = std::static_pointer_cast<box2d::Body>(body);
				m_impl->DestroyBody(b2_body->GetImpl());
				itr = m_bodies.erase(itr);
				break;
			}
			else
			{
				++itr;
			}
		}
	}
	m_destroy_bodies.clear();

	for (auto& joint : m_destroy_joints)
	{
		for (auto itr = m_joints.begin(); itr != m_joints.end(); )
		{
			if (*itr == joint)
			{
				auto b2_joint = std::static_pointer_cast<box2d::Joint>(joint);
				m_impl->DestroyJoint(b2_joint->GetImpl());
				itr = m_joints.erase(itr);
				break;
			}
			else
			{
				++itr;
			}
		}
	}
	m_destroy_bodies.clear();
}

}
}
}