#pragma once

#include "uniphysics/rigid/DebugDraw.h"

#include <box2d/b2_draw.h>

namespace up
{
namespace rigid
{
namespace box2d
{

class DebugDraw : public b2Draw, public rigid::DebugDraw
{
public:
	DebugDraw();

	virtual void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;

	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	virtual void DrawTransform(const b2Transform& xf) override;

	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

private:
	static uint32_t TransColor(const b2Color& color);

	static std::vector<sm::vec2> TransVertices(const b2Vec2* vertices, int vertexCount);

}; // DebugDraw

}
}
}