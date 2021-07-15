#pragma once

#include "uniphysics/rigid/World.h"

#include <vector>

class b2World;

namespace up
{
namespace rigid
{

class DebugDraw;

namespace box2d
{

class Body;

class World : public rigid::World
{
public:
    World();

    virtual void AddBody(const std::shared_ptr<rigid::Body>& body) override;
    virtual void RemoveBody(const std::shared_ptr<rigid::Body>& body) override;

    virtual void StepSimulation(float dt) override;

    virtual Picked PickBody(const sm::vec3& ray_from, const sm::vec3& ray_to) const override;
    virtual bool MovePickedBody(const Picked& picked, const sm::vec3& ray_from, const sm::vec3& ray_to) override;

    virtual void RemoveConstraint(const std::shared_ptr<rigid::Constraint>& cons) override;

    void SetDebugDraw(rigid::DebugDraw& draw);
    void DebugDraw() const;

private:
    void PreSimulation();

private:
    std::shared_ptr<b2World> m_impl = nullptr;

    std::vector<std::shared_ptr<Body>> m_bodies;
    
    int m_velocity_iter = 8;
    int m_position_iter = 3;

    std::vector<std::shared_ptr<rigid::Body>> m_rm_list;

}; // World

}
}
}