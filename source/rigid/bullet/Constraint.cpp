#include "uniphysics/rigid/bullet/Constraint.h"

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

namespace up
{
namespace rigid
{
namespace bullet
{

Constraint::Constraint(std::unique_ptr<btTypedConstraint> impl)
    : m_impl(std::move(impl))
{
}

Constraint::~Constraint()
{
}

}
}
}