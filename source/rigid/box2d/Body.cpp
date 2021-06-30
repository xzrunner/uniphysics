#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/Shape.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

namespace up
{
namespace rigid
{
namespace box2d
{

Body::Body()
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

void Body::SetImpl(b2Body* impl)
{
	m_impl = impl;

	for (auto& shape : m_fixtures) 
	{
		auto _shape = shape->GetImpl();;
		if (!_shape) {
			continue;
		}

		b2FixtureDef fd;
		fd.density = 1.0;
		fd.shape = _shape;

		m_impl->CreateFixture(&fd);
	}
}

}
}
}