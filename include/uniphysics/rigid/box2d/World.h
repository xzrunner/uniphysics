#pragma once

#include "uniphysics/rigid/World.h"

#include <vector>
#include <functional>

class b2World;

namespace up
{
namespace rigid
{

class DebugDraw;

namespace box2d
{

class Body;
class Joint;
class ContactListener;

class World : public rigid::World
{
public:
    World();
    virtual ~World();

    virtual void AddBody(const std::shared_ptr<rigid::Body>& body) override;
    virtual void RemoveBody(const std::shared_ptr<rigid::Body>& body) override;

    virtual void StepSimulation(float dt) override;

    virtual Picked PickBody(const sm::vec3& ray_from, const sm::vec3& ray_to) const override;
    virtual bool MovePickedBody(const Picked& picked, const sm::vec3& ray_from, const sm::vec3& ray_to) override;

    virtual void RemoveConstraint(const std::shared_ptr<rigid::Constraint>& cons) override;

    void AddJoint(const std::shared_ptr<Joint>& joint);
    void RemoveJoint(const std::shared_ptr<Joint>& joint);

    void SetDebugDraw(rigid::DebugDraw& draw);
    void DebugDraw() const;

    struct Callback
    {
        std::function<void(const std::shared_ptr<Body>&, const std::shared_ptr<Body>&)> begin_contact;
    };
    static void RegisterCallback(const Callback& cb);

private:
    void PreSimulation();

private:
    b2World* m_impl = nullptr;

    std::vector<std::shared_ptr<Body>>  m_bodies;
    std::vector<std::shared_ptr<Joint>> m_joints;

    int m_velocity_iter = 8;
    int m_position_iter = 3;

    std::unique_ptr<ContactListener> m_contact_lsn = nullptr;

    std::vector<std::shared_ptr<rigid::Body>>  m_destroy_bodies;
    std::vector<std::shared_ptr<Joint>> m_destroy_joints;

    friend class ContactListener;

}; // World

}
}
}