#include "stdafx.h"

#include "RigidBodyComponent.h"

#include "GameObject.h"

#include "EventBus.h"
#include "PhysicsComponentEvents.h"
#include "VectorAlgebra3D.inl"

RigidBodyComponent::RigidBodyComponent(GameObject& gameObject, const float mass)
	: IComponent(gameObject)
	, m_mass(mass)
	, m_invMass((mass > 0.0f) ? 1.0f/mass : 0.0f)
{
	EventBus::getInstance().FireEvent(std::make_shared<RigidBodyCreateEvent>(*this));
}
void RigidBodyComponent::InverseVelocity()
{
	const float temp = m_velocity.x *m_velocity.x + m_velocity.y + m_velocity.x;
	m_velocity.x = m_velocity.x / abs(temp);
	m_velocity.y = m_velocity.y / abs(temp);
}

void RigidBodyComponent::AddVelocity(const Vector2f & velocity)
{
	if(isOnGround)
	{
		m_velocity.x *= 0.94f;
	}
	else
	{
		m_velocity.x *= 0.95f;
	}
	m_velocity += velocity;
}

void RigidBodyComponent::PhysicsUpdate(float fDeltaTime)
{	
	m_gameObject.move(m_velocity * fDeltaTime + m_correction);
	m_correction.x = m_correction.y = 0.0f;
}

void RigidBodyComponent::AddPositionalCorrection(const Vector2f& correction)
{
	m_correction = correction;
	
}
