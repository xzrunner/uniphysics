#pragma once

#include <boost/noncopyable.hpp>

#include <SM_Vector.h>

#include <memory>

namespace up
{
namespace cloth
{

class Cloth : boost::noncopyable
{
public:
    virtual ~Cloth() {}

    virtual void SetGravity(const sm::vec3& gravity) = 0;
    virtual sm::vec3 GetGravity() const = 0;

}; // Cloth

}
}