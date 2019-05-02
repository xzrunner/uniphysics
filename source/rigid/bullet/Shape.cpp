#include "uniphysics/rigid/bullet/Shape.h"
#include "uniphysics/rigid/bullet/Utility.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace up
{
namespace rigid
{
namespace bullet
{

Shape::Shape()
{
}

Shape::~Shape()
{
}

void Shape::InitBoxShape(const sm::vec3& half_extents)
{
    m_impl = std::make_unique<btBoxShape>(vec3_sm2bt(half_extents));
}

}
}
}