#include "uniphysics/rigid/box2d/World.h"
#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/DebugDraw.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_contact.h>

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
			if (body->GetBody() == b) {
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
	m_impl = std::make_shared<b2World>(b2gravity);
	m_contact_lsn = std::make_unique<ContactListener>(*this);
	m_impl->SetContactListener(m_contact_lsn.get());
}

void World::AddBody(const std::shared_ptr<rigid::Body>& body)
{
	auto b2_body = std::static_pointer_cast<box2d::Body>(body);
	b2_body->CreateBody(m_impl);

	m_bodies.push_back(b2_body);
}

void World::RemoveBody(const std::shared_ptr<rigid::Body>& body)
{
	if (m_impl->IsLocked()) {
		m_rm_list.push_back(body);
		return;
	}

	for (auto itr = m_bodies.begin(); itr != m_bodies.end(); )
	{
		if (*itr == body) 
		{
			auto b2_body = std::static_pointer_cast<box2d::Body>(body);
			m_impl->DestroyBody(b2_body->GetBody());
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
	for (auto& rm : m_rm_list)
	{
		for (auto itr = m_bodies.begin(); itr != m_bodies.end(); )
		{
			if (*itr == rm)
			{
				auto b2_body = std::static_pointer_cast<box2d::Body>(rm);
				m_impl->DestroyBody(b2_body->GetBody());
				itr = m_bodies.erase(itr);
				break;
			}
			else
			{
				++itr;
			}
		}
	}
	m_rm_list.clear();
}

}
}
}