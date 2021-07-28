#pragma once

#include <memory>

class b2Body;

namespace up
{
namespace rigid
{
namespace box2d
{

class Shape;

class Fixture
{
public:
	Fixture(const std::shared_ptr<Shape>& shape);

	void SetDensity(float density) { m_density = density; }
	void SetRestitution(float restitution) { m_restitution = restitution; }
	void SetFriction(float friction) { m_friction = friction; }

	void SetFilter(uint16_t category_bits, uint16_t mask_bits, int16_t group_index) {
		m_category_bits = category_bits;
		m_mask_bits = mask_bits;
		m_group_index = group_index;
	}

	void Build(b2Body* body);

private:
	std::shared_ptr<Shape> m_shape = nullptr;

    float m_density     = 0.0f;
    float m_restitution = 0.0f;
    float m_friction    = 0.0f;

	// filter
	uint16_t m_category_bits = 0x0001;
	uint16_t m_mask_bits     = 0xFFFF;
	int16_t  m_group_index   = 0;

}; // Fixture

}
}
}