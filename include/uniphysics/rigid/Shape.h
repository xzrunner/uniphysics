#pragma once

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

namespace up
{
namespace rigid
{

class Shape : boost::noncopyable
{
public:
    virtual ~Shape() {}

    virtual void InitBoxShape(const sm::vec3& half_extents) = 0;

}; // Shape

}
}