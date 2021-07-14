#pragma once

#include "uniphysics/rigid/Body.h"

#include <memory>
#include <vector>
#include <string>

class b2Body;
class b2World;

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
    Body(const std::string& type);
    virtual ~Body();

    virtual sm::vec3 GetPosition() const override;

    virtual void ForceActivationState(int state) override;

    virtual void Activate() override;

    void AddShape(const std::shared_ptr<Shape>& shape) {
        m_fixtures.push_back(shape);
    }

    void CreateBody(const std::shared_ptr<b2World>& world);

private:
    std::shared_ptr<b2World> m_world = nullptr;

    b2Body* m_impl = nullptr;
  
    std::vector<std::shared_ptr<Shape>> m_fixtures;

    std::string m_type;

}; // Body

}
}
}