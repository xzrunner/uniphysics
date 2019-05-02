#pragma once

#include <boost/noncopyable.hpp>

namespace up
{
namespace rigid
{

class Constraint : boost::noncopyable
{
public:
    virtual ~Constraint() {}

}; // Constraint

}
}