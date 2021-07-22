#pragma once

#include <SM_Vector.h>

#include <memory>

class b2World;
class b2Joint;

namespace up
{
namespace rigid
{
namespace box2d
{

class Body;

enum class JointType
{
	Unknown,
	Revolute,
	Prismatic,
	Distance,
	Pulley,
	Mouse,
	Gear,
	Wheel,
	Weld,
	Friction,
	Rope,
	Motor
};

class Joint
{
public:
	Joint(JointType type, const std::shared_ptr<Body>& body_a,
		const std::shared_ptr<Body>& body_b);
	virtual ~Joint() {}

	auto GetType() const { return m_type; }

	auto GetBodyA() const { return m_body_a; }
	auto GetBodyB() const { return m_body_b; }

	void SetImpl(b2Joint* joint);
	auto GetImpl() const { return m_impl; }
	void DeleteImpl(b2World* world);

private:
	JointType m_type = JointType::Unknown;

	std::shared_ptr<Body> m_body_a = nullptr;
	std::shared_ptr<Body> m_body_b = nullptr;

	b2Joint* m_impl = nullptr;

}; // Joint

class RevoluteJoint : public Joint
{
public:
	RevoluteJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
		const sm::vec2& anchor);

	auto& GetAnchor() const { return m_anchor; }

	void SetAngleLimit(bool enable_limit, float lower, float upper) {
		m_enable_limit = enable_limit;
		m_angle_lower = lower;
		m_angle_upper = upper;
	}
	void GetAngleLimit(bool& enable_limit, float& lower, float& upper) {
		enable_limit = m_enable_limit;
		lower = m_angle_lower;
		upper = m_angle_upper;
	}

	void SetMotor(bool enable_motor, float max_torque, float speed) {
		m_enable_motor = enable_motor;
		m_max_motor_torque = max_torque;
		m_motor_speed = speed;
	}
	void GetMotor(bool& enable_motor, float& max_torque, float& speed) {
		enable_motor = m_enable_motor;
		max_torque = m_max_motor_torque;
		speed = m_motor_speed;
	}

private:
	sm::vec2 m_anchor;

	// rotate
	bool m_enable_limit = false;
	float m_angle_lower = 0;
	float m_angle_upper = 0;

	// motor
	bool m_enable_motor = false;
	float m_max_motor_torque = 0;
	float m_motor_speed = 0;

}; // RevoluteJoint

class PrismaticJoint : public Joint
{
public:
	PrismaticJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
		const sm::vec2& anchor, const sm::vec2& axis);

	auto& GetAnchor() const { return m_anchor; }
	auto& GetAxis() const { return m_axis; }

	void SetTranslateRegion(float lower, float upper) { m_lower = lower; m_upper = upper; }
	void GetTranslateRegion(float& lower, float& upper) { lower = m_lower; upper = m_upper; }

private:
	sm::vec2 m_anchor;
	sm::vec2 m_axis;

	float m_lower = 0;
	float m_upper = 0;

}; // PrismaticJoint

class DistanceJoint : public Joint
{
public:
	DistanceJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
		const sm::vec2& anchor_a, const sm::vec2& anchor_b);

	auto& GetAnchorA() const { return m_anchor_a; }
	auto& GetAnchorB() const { return m_anchor_b; }

	void SetLength(float min, float max) { m_min_length = min; m_max_length = max; }
	void GetLength(float& min, float& max) const { min = m_min_length; max = m_max_length; }

private:
	sm::vec2 m_anchor_a, m_anchor_b;

	float m_min_length, m_max_length;

}; // DistanceJoint

class MouseJoint : public Joint
{
public:
	MouseJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
		const sm::vec2& target, float max_force);

	auto& GetTarget() const { return m_target; }
	auto& GetMaxForce() const { return m_max_force; }

	void SetTarget(const sm::vec2& target);

private:
	sm::vec2 m_target;
	float m_max_force;

}; // MouseJoint

class WheelJoint : public Joint
{
public:
	WheelJoint(const std::shared_ptr<Body>& body_a, const std::shared_ptr<Body>& body_b,
		const sm::vec2& anchor, const sm::vec2& axis);

	auto& GetAnchor() const { return m_anchor; }
	auto& GetAxis() const { return m_axis; }

	void SetTranslateLimit(bool enable_limit, float lower, float upper) {
		m_enable_limit = enable_limit;
		m_translate_lower = lower;
		m_translate_upper = upper;
	}
	void GetTranslateLimit(bool& enable_limit, float& lower, float& upper) {
		enable_limit = m_enable_limit;
		lower = m_translate_lower;
		upper = m_translate_upper;
	}

	void SetMotor(bool enable_motor, float max_torque, float speed) {
		m_enable_motor = enable_motor;
		m_max_motor_torque = max_torque;
		m_motor_speed = speed;
	}
	void GetMotor(bool& enable_motor, float& max_torque, float& speed) {
		enable_motor = m_enable_motor;
		max_torque = m_max_motor_torque;
		speed = m_motor_speed;
	}

	void SetSuspension(float stiffness, float damping) {
		m_stiffness = stiffness;
		m_damping = damping;
	}
	void GetSuspension(float& stiffness, float& damping) {
		stiffness = m_stiffness;
		damping = m_damping;
	}

	void SetMotorSpeed(float speed);

private:
	sm::vec2 m_anchor;
	sm::vec2 m_axis;

	// translate
	bool m_enable_limit = false;
	float m_translate_lower = 0;
	float m_translate_upper = 0;

	// motor
	bool m_enable_motor = false;
	float m_max_motor_torque = 0;
	float m_motor_speed = 0;

	// suspension
	float m_stiffness = 0;
	float m_damping = 0;

}; // WheelJoint

}
}
}