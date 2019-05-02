#pragma once

#include "uniphysics/rigid/Constraint.h"

#include <memory>

class btTypedConstraint;

namespace up
{
namespace rigid
{
namespace bullet
{

class Constraint : public rigid::Constraint
{
public:
    Constraint(std::unique_ptr<btTypedConstraint> impl);
    virtual ~Constraint();

    auto& GetImpl() const { return m_impl; }

private:
    std::unique_ptr<btTypedConstraint> m_impl = nullptr;

}; // Constraint

}
}
}