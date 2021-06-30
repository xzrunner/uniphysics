#pragma once

#include "uniphysics/noncopyable.h"

#include <SM_Vector.h>

namespace up
{
namespace rigid
{

class Body : noncopyable
{
public:
    virtual ~Body() {}

    virtual sm::vec3 GetPosition() const = 0;

    virtual void ForceActivationState(int state) = 0;

    virtual void Activate() = 0;

}; // Body

}
}