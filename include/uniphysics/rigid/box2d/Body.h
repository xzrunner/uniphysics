#pragma once

#include "uniphysics/rigid/Body.h"

#include <memory>
#include <vector>

class b2Body;

namespace up
{
namespace rigid
{
namespace box2d
{

class World;
class Shape;

class Body : public rigid::Body
{
public:
    Body();

    virtual sm::vec3 GetPosition() const override;

    virtual void ForceActivationState(int state) override;

    virtual void Activate() override;

    void AddShape(const std::shared_ptr<Shape>& shape) {
        m_fixtures.push_back(shape);
    }

    void SetImpl(b2Body* impl);

private:
    b2Body* m_impl = nullptr;
  
    std::vector<std::shared_ptr<Shape>> m_fixtures;

}; // Body

}
}
}