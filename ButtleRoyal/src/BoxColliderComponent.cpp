#include "stdafx.h"

#include "BoxColliderComponent.h"
#include "GameObject.h"

#include "EventBus.h"
#include "PhysicsComponentEvents.h"

BoxColliderComponent::BoxColliderComponent(GameObject &gameObject,
	RigidBodyComponent& body, const FloatRect& AABB, const bool isTrigger)
	: IComponent(gameObject)
	, m_body(body)
	, m_isTrigger(isTrigger)
	, m_AABB(AABB)
	, m_transformedAABB(AABB)
{
	// send event that collider was created (e.g., to notify physics)
	const Transform& transform = m_gameObject.getTransform();
	m_transformedAABB = transform.transformRect(m_AABB);
	EventBus::getInstance().FireEvent(std::make_shared<BoxColliderCreateEvent>(*this));
}

void 
BoxColliderComponent::VUpdate(float fDeltaTime)
{
	const Transform& transform = m_gameObject.getTransform();
	m_transformedAABB = transform.transformRect(m_AABB);
}

void 
BoxColliderComponent::registerOnCollisionFunction(const OnCollisionFunction & func)
{
	m_onCollisionFunctions.push_back(func);
}

void 
BoxColliderComponent::OnCollision(BoxColliderComponent& collider)
{
	for (auto f : m_onCollisionFunctions)
		f(*this, collider);
}
