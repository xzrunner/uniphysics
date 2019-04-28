#pragma once

#include <boost/noncopyable.hpp>

#include <memory>

namespace up
{
namespace rigid
{

class Body;

class World : boost::noncopyable
{
public:
    virtual ~World() {}

    virtual void AddBody(const std::shared_ptr<Body>& body) = 0;

    virtual void StepSimulation(float dt) = 0;

}; // World

}
}