#pragma once

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

#include <memory>

namespace up
{
namespace rigid
{

class Body;
class Constraint;

class World : boost::noncopyable
{
public:
    virtual ~World() {}

    virtual void AddBody(const std::shared_ptr<Body>& body) = 0;

    virtual void StepSimulation(float dt) = 0;

    struct Picked
    {
        std::shared_ptr<rigid::Body>       body = nullptr;
        std::shared_ptr<rigid::Constraint> cons = nullptr;
        int saved_state = 0;

        sm::vec3 old_picking_pos;
        sm::vec3 hit_pos;
        float    old_picking_dist = 0;

        Picked()
        {
            old_picking_pos.MakeInvalid();
            hit_pos.MakeInvalid();
        }

        void Reset()
        {
            body.reset();
            cons.reset();
            saved_state = 0;

            old_picking_pos.MakeInvalid();
            hit_pos.MakeInvalid();
            old_picking_dist = 0;
        }
    };
    virtual Picked PickBody(const sm::vec3& ray_from, const sm::vec3& ray_to) const = 0;
    virtual bool MovePickedBody(const Picked& picked, const sm::vec3& ray_from, const sm::vec3& ray_to) = 0;

    virtual void RemoveConstraint(const std::shared_ptr<rigid::Constraint>& cons) = 0;

}; // World

}
}