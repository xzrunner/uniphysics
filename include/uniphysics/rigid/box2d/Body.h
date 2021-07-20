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
    Body(const std::string& type, int flag = -1);

    virtual sm::vec3 GetPosition() const override;

    virtual void ForceActivationState(int state) override;

    virtual void Activate() override;

    void AddShape(const std::shared_ptr<Shape>& shape) {
        m_fixtures.push_back(shape);
    }

    auto& GetType() const { return m_type; }
    auto GetFlag() const { return m_flag; }

    void SetGravityScale(float gravity) { m_gravity = gravity; }

    void SetDensity(float density) { m_density = density; }
    void SetRestitution(float restitution) { m_restitution = restitution; }
    void SetFriction(float friction) { m_friction = friction; }

    void ApplyForce(const sm::vec2& force);
    void ApplyTorque(float torque);
    void ApplyLinearImpulse(const sm::vec2& impulse);
    void ApplyAngularImpulse(float impulse);

    void SetLinearVelocity(const sm::vec2& velocity);
    sm::vec2 GetLinearVelocity() const;

    void SetTransform(const sm::vec2& pos, float angle);

    void SetImpl(b2Body* body);
    auto GetImpl() const { return m_impl; }
    void DeleteImpl(b2World* world);

    float GetMass() const;
    float GetAngle() const;

private:
    b2Body* m_impl = nullptr;
  
    std::vector<std::shared_ptr<Shape>> m_fixtures;

    std::string m_type;

    float m_gravity = 1.0f;

    float m_density     = 0.0f;
    float m_restitution = 0.0f;
    float m_friction    = 0.0f;

    int m_flag = -1;

}; // Body

}
}
}