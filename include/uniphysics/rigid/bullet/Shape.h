#pragma once

#include "uniphysics/rigid/Shape.h"

#include <memory>

class btCollisionShape;

namespace up
{
namespace rigid
{
namespace bullet
{

class Shape : public rigid::Shape
{
public:
    Shape();
    virtual ~Shape();

    virtual void InitBoxShape(const sm::vec3& half_extents) override;

    auto& GetImpl() const { return m_impl; }

private:
    std::unique_ptr<btCollisionShape> m_impl = nullptr;

}; // Shape

}
}
}