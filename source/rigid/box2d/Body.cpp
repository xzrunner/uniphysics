#include "uniphysics/rigid/box2d/Body.h"
#include "uniphysics/rigid/box2d/Shape.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace up
{
namespace rigid
{
namespace box2d
{

Body::Body(const std::string& type)
	: m_type(type)
{
}

Body::~Body()
{
	if (m_impl) {
		m_world->DestroyBody(m_impl);
		m_impl = NULL;
	}
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

void Body::CreateBody(const std::shared_ptr<b2World>& world)
{
	m_world = world;

	b2BodyDef bd;
	if (m_type == "static") {
		bd.type = b2_staticBody;
	} else if (m_type == "kinematic") {
		bd.type = b2_kinematicBody;
	} else if (m_type == "dynamic") {
		bd.type = b2_dynamicBody;
	}

	m_impl = m_world->CreateBody(&bd);

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