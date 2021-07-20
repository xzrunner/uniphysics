#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/Shape.h"

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
	m_impl->SetTransform(b2Vec2(pos.x, pos.y), angle);
}

void Body::SetImpl(b2Body* body)
{
	assert(!m_impl);
	if (m_impl) {
		throw std::runtime_error("Body not empty.");
	}

	m_impl = body;

	for (auto& shape : m_fixtures) 
	{
		auto _shape = shape->GetImpl();;
		if (!_shape) {
			continue;
		}
	
		b2FixtureDef fd;
		fd.shape = _shape;

		fd.density = m_density;
		fd.restitution = m_restitution;
		fd.friction = m_friction;

		m_impl->SetGravityScale(m_gravity);

		m_impl->CreateFixture(&fd);
	}
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