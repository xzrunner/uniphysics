#include "uniphysics/rigid/box2d/Fixture.h"
#include "uniphysics/rigid/box2d/Shape.h"

#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>

namespace up
{
namespace rigid
{
namespace box2d
{

Fixture::Fixture(const std::shared_ptr<Shape>& shape)
	: m_shape(shape)
{
}

void Fixture::Build(b2Body* body)
{
	if (!m_shape) {
		return;
	}

	auto shape = m_shape->GetImpl();;
	if (!shape) {
		return;
	}

	b2FixtureDef fd;
	fd.shape = shape;

	fd.density = m_density;
	fd.restitution = m_restitution;
	fd.friction = m_friction;

	fd.filter.categoryBits = m_category_bits;
	fd.filter.maskBits     = m_mask_bits;
	fd.filter.groupIndex   = m_group_index;

	body->CreateFixture(&fd);
}

}
}
}