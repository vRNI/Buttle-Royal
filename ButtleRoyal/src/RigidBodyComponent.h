#pragma once

#include "IComponent.h"

class RigidBodyComponent : public IComponent
{
public:

	RigidBodyComponent(GameObject& gameObject, float mass);

	Vector2f GetVelocity() const { return m_velocity; }
	void SetVelocity(const Vector2f vel) { m_velocity = vel; }
	void AddVelocity(const Vector2f& velocity);
	void changeIsOnGround(const bool value) { isOnGround = value; }

	virtual bool VInit() override { return true; }
	virtual void VUpdate(float fDeltaTime) override {};

	void PhysicsUpdate(float fDeltaTime);

	float GetMass() const { return m_mass; }
	float GetInvMass() const { return m_invMass; }

	void AddPositionalCorrection(const Vector2f& correction);

	void InverseVelocity();
	//std::vector<sf::Vector2f> GetForces() { return  m_forces; };
	//void PushBackForce(sf::Vector2f force) { m_forces.push_back(force); }
	//void PushBackImpulse(sf::Vector2f impulse) { m_impulses.push_back(impulse); }
	//std::vector<sf::Vector2f> GetImpulses() { return  m_impulses; };
	//void ClearImpulses() { m_impulses.clear(); }
	Vector2f& GetAcceleration() { return  m_acceleration; };
	Vector2f& GetVelocity() { return m_velocity; };
private:

	Vector2f m_velocity;
	Vector2f m_acceleration;

	bool isOnGround;

	//std::vector<sf::Vector2f>m_forces;
	//std::vector<sf::Vector2f>m_impulses;

	float m_mass;
	float m_invMass;
	
	Vector2f m_correction;
};
