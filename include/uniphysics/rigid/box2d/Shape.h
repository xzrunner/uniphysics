#pragma once

#include "uniphysics/rigid/Shape.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>
#include <memory>

class b2Shape;

namespace up
{
namespace rigid
{
namespace box2d
{

class Shape : public rigid::Shape
{
public:
    Shape();

    void InitEdgeShape(const sm::vec2& p0, const sm::vec2& p1);
    void InitRectShape(const sm::rect& rect);
    void InitCircleShape(const sm::vec2& center, float radius);
    void InitChainShape(const std::vector<sm::vec2>& vertices, bool is_closed);
    void InitPolygonShape(const std::vector<sm::vec2>& vertices);

    b2Shape* GetImpl() const { return m_impl.get(); }

private:
    std::unique_ptr<b2Shape> m_impl = nullptr;

}; // Shape

}
}
}