#include "uniphysics/rigid/box2d/Shape.h"
#include "uniphysics/rigid/box2d/config.h"

#include <box2d/b2_math.h>
#include <box2d/b2_chain_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_edge_shape.h>

namespace up
{
namespace rigid
{
namespace box2d
{

Shape::Shape()
{
}

void Shape::InitEdgeShape(const sm::vec2& p0, const sm::vec2& p1)
{
	auto shape = std::make_unique<b2EdgeShape>();
	b2Vec2 b2_p0 = b2Vec2(p0.x / SCALE_FACTOR, p0.y / SCALE_FACTOR);
	b2Vec2 b2_p1 = b2Vec2(p1.x / SCALE_FACTOR, p1.y / SCALE_FACTOR);
	shape->SetTwoSided(b2_p0, b2_p1);

	m_impl = std::move(shape);
}

void Shape::InitRectShape(const sm::rect& rect)
{
	const float hx = (rect.xmax - rect.xmin) * 0.5f / SCALE_FACTOR,
		        hy = (rect.ymax - rect.ymin) * 0.5f / SCALE_FACTOR;
	const float cx = (rect.xmax + rect.xmin) * 0.5f / SCALE_FACTOR,
		        cy = (rect.ymax + rect.ymin) * 0.5f / SCALE_FACTOR;

	auto shape = std::make_unique<b2PolygonShape>();
	shape->SetAsBox(hx, hy, b2Vec2(cx, cy), 0);

	m_impl = std::move(shape);
}

void Shape::InitCircleShape(const sm::vec2& center, float radius)
{
	auto shape = std::make_unique<b2CircleShape>();
	shape->m_radius = radius / SCALE_FACTOR;
	shape->m_p.x = center.x / SCALE_FACTOR;
	shape->m_p.y = center.y / SCALE_FACTOR;

	m_impl = std::move(shape);
}

void Shape::InitChainShape(const std::vector<sm::vec2>& vertices, bool is_closed)
{
	const size_t size = vertices.size();
	std::vector<b2Vec2> dst(size);
	for (size_t i = 0; i < size; ++i)
	{
		dst[i].x = vertices[i].x / SCALE_FACTOR;
		dst[i].y = vertices[i].y / SCALE_FACTOR;
	}

	auto shape = std::make_unique<b2ChainShape>();
	if (is_closed) {
		shape->CreateLoop(&dst[0], size);
	} else {
		shape->CreateChain(&dst[0], size, dst.back(), dst.front());
	}

	m_impl = std::move(shape);
}

void Shape::InitPolygonShape(const std::vector<sm::vec2>& vertices)
{
	const size_t size = vertices.size();
	std::vector<b2Vec2> dst(size);
	for (size_t i = 0; i < size; ++i)
	{
		dst[i].x = vertices[i].x / SCALE_FACTOR;
		dst[i].y = vertices[i].y / SCALE_FACTOR;
	}

	auto shape = std::make_unique<b2PolygonShape>();
	shape->Set(dst.data(), dst.size());

	m_impl = std::move(shape);
}

}
}
}