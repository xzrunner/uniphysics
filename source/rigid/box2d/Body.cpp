#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/Shape.h"
#include "uniphysics/rigid/box2d/Fixture.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

#include <stdexcept>

namespace up
{
namespace rigid
{
namespace box2d
{

Body::Body(const std::string& type, int flag)
	: m_type(type)
	, m_flag(flag)
{
}

sm::vec3 Body::GetPosition() const
{
	if (m_impl) 
	{
		auto& pos = m_impl->GetPosition();
		return sm::vec3(pos.x, pos.y, 0.0);
	}
	else
	{
		return sm::vec3(0, 0, 0);
	}
}

void Body::ForceActivationState(int state)
{

}

void Body::Activate()
{

}

void Body::AddFixture(const std::shared_ptr<Shape>& shape,
	                  int category, const std::vector<int>& not_collide)
{
	auto fixture = std::make_shared<Fixture>(shape);

	uint16_t category_bits = 0x1 << category;

	uint16_t mask_bits = 0xFFFF;
	for (auto category : not_collide) {
		mask_bits = mask_bits & ~(0x1 << category);
	}

	int16_t group_index = 0;

	fixture->SetFilter(category_bits, mask_bits, group_index);

	m_fixtures.push_back(fixture);
}

void Body::ApplyForce(const sm::vec2& force)
{
	m_impl->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void Body::ApplyTorque(float torque)
{
	m_impl->ApplyTorque(torque, true);
}

void Body::ApplyLinearImpulse(const sm::vec2& impulse)
{
	m_impl->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
}

void Body::ApplyAngularImpulse(float impulse)
{
	m_impl->ApplyAngularImpulse(impulse, true);
}

void Body::SetLinearVelocity(const sm::vec2& velocity)
{
	m_impl->SetLinearVelocity({ velocity.x, velocity.y });
}

sm::vec2 Body::GetLinearVelocity() const
{
	auto& b2 = m_impl->GetLinearVelocity();
	return { b2.x, b2.y };
}

void Body::SetTransform(const sm::vec2& pos, float angle)
{
	if (m_impl) {
		m_impl->SetTransform(b2Vec2(pos.x, pos.y), angle);
	} else {
		m_pos = pos;
		m_angle = angle;
	}
}

void Body::SetImpl(b2Body* body)
{
	assert(!m_impl);
	if (m_impl) {
		throw std::runtime_error("Body not empty.");
	}

	m_impl = body;

	for (auto& f : m_fixtures) 
	{
		f->SetDensity(m_density);
		f->SetRestitution(m_restitution);
		f->SetFriction(m_friction);

		f->Build(m_impl);
	}
	m_impl->SetGravityScale(m_gravity);

	m_impl->SetTransform(b2Vec2(m_pos.x, m_pos.y), m_angle);
}

void Body::DeleteImpl(b2World* world)
{
	if (m_impl) {
		world->DestroyBody(m_impl);
		m_impl = nullptr;
	}
}

float Body::GetMass() const
{
	return m_impl ? m_impl->GetMass() : 0.0f;
}

float Body::GetAngle() const
{
	return m_impl ? m_impl->GetAngle() : 0.0f;
}

}
}
}