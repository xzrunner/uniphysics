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

}
}
}