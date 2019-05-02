#pragma once

#include "uniphysics/rigid/Body.h"

#include <SM_Vector.h>

#include <memory>

class btRigidBody;

namespace up
{
namespace rigid
{

class Shape;

namespace bullet
{

class Body : public rigid::Body
{
public:
    Body(float mass, const sm::vec3& pos, const std::shared_ptr<rigid::Shape>& shape,
        const sm::vec4& color = sm::vec4(1, 0, 0, 1));

    virtual sm::vec3 GetPosition() const override;

    virtual void ForceActivationState(int state) override;

    virtual void Activate() override;

    auto& GetImpl() { return m_impl; }

private:
    std::unique_ptr<btRigidBody> m_impl = nullptr;

    std::shared_ptr<rigid::Shape> m_shape = nullptr;

}; // Body

}
}
}