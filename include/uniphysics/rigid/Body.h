#pragma once

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

namespace up
{
namespace rigid
{

class Body : boost::noncopyable
{
public:
    virtual ~Body() {}

    virtual sm::vec3 GetPosition() const = 0;

}; // Body

}
}