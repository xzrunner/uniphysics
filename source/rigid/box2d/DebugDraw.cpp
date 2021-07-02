#include "uniphysics/rigid/box2d/DebugDraw.h"
#include "uniphysics/rigid/box2d/config.h"

namespace up
{
namespace rigid
{
namespace box2d
{

DebugDraw::DebugDraw()
{
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	SetFlags(flags);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	auto _vertices = TransVertices(vertices, vertexCount);
	m_pt.AddPolyline(_vertices.data(), _vertices.size(), TransColor(color), 2.0f);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	auto _vertices = TransVertices(vertices, vertexCount);
	m_pt.AddPolygonFilled(_vertices.data(), _vertices.size(), TransColor(color));
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	m_pt.AddCircle(sm::vec2(center.x * SCALE_FACTOR, center.y * SCALE_FACTOR), radius * SCALE_FACTOR, TransColor(color), 2.0f, 16);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	m_pt.AddCircleFilled(sm::vec2(center.x * SCALE_FACTOR, center.y * SCALE_FACTOR), radius * SCALE_FACTOR, TransColor(color), 16);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_pt.AddLine(
		sm::vec2(p1.x * SCALE_FACTOR, p1.y * SCALE_FACTOR),
		sm::vec2(p2.x * SCALE_FACTOR, p2.y * SCALE_FACTOR),
		TransColor(color), 2.0f
	);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1, 0, 0));

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0, 1, 0));
}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	m_pt.AddCircleFilled(sm::vec2(p.x * SCALE_FACTOR, p.y * SCALE_FACTOR), size, TransColor(color), 16);
}

uint32_t DebugDraw::TransColor(const b2Color& color)
{
	uint8_t r = color.r * 255;
	uint8_t g = color.g * 255;
	uint8_t b = color.b * 255;
	uint8_t a = color.a * 255;
	return a << 24 | b << 16 | g << 8 | r;
}

std::vector<sm::vec2> DebugDraw::TransVertices(const b2Vec2* vertices, int vertexCount)
{
	std::vector<sm::vec2> ret;
	ret.reserve(vertexCount);
	for (int i = 0; i < vertexCount; ++i) {
		ret.emplace_back(vertices[i].x * SCALE_FACTOR, vertices[i].y * SCALE_FACTOR);
	}
	return ret;
}

}
}
}