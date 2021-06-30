#pragma once

#include "uniphysics/noncopyable.h"

#include <SM_Vector.h>

namespace up
{
namespace rigid
{

class Shape : noncopyable
{
public:
    virtual ~Shape() {}

    virtual void InitBoxShape(const sm::vec3& half_extents) = 0;

}; // Shape

}
}