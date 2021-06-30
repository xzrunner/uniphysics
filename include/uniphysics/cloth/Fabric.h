#pragma once

#include "uniphysics/noncopyable.h"

#include <memory>

namespace up
{
namespace cloth
{

class Fabric : noncopyable
{
public:
    virtual ~Fabric() {}

    virtual uint32_t GetNumPhases() const = 0;

}; // Fabric

}
}