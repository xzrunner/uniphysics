#pragma once

#include <boost/noncopyable.hpp>

#include <memory>

namespace up
{
namespace cloth
{

class Fabric : boost::noncopyable
{
public:
    virtual ~Fabric() {}

    virtual uint32_t GetNumPhases() const = 0;

}; // Fabric

}
}