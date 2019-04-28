#include "uniphysics/rigid/bullet/Shape.h"

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
    btVector3 bt_he(half_extents.x, half_extents.y, half_extents.z);
    m_impl = std::make_unique<btBoxShape>(bt_he);
}

}
}
}