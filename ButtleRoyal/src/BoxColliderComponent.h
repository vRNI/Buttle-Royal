#pragma once

#include <functional>
#include <list>

#include "IComponent.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"

class BoxColliderComponent : public IComponent
{
public:
	/// \param collider1 the collider for which OnCollision was called.
	/// \param collider2 the collider with which the collision happened.
	typedef std::function<void(BoxColliderComponent& collider1, 
		BoxColliderComponent& collider2)> OnCollisionFunction;

	/// \param body the rigid body the collider is attached to.
	/// \param rect the size of the AABB in pixel
	BoxColliderComponent(GameObject &gameObject, RigidBodyComponent& body,
		const FloatRect& AABB,bool isTrigger);

	bool VInit() override { return true; }
	void VUpdate(float fDeltaTime) override;

	/// Add delegate function to be executed when collision is detected.
	/// Signature: void func(BoxColliderComponent&)
	void registerOnCollisionFunction(const  OnCollisionFunction& func);

	/// Method called when collision occured. Method calls all subscribed
	/// OnCollisionFunctions
	///
	/// \param collider collision occured with this collider
	void OnCollision(BoxColliderComponent& collider);

	sf::FloatRect getTransformedAABB() const { return m_transformedAABB; }
	sf::FloatRect getAABB() const { return m_AABB; }
	
	GameObject GetGameObject()
	{
		return m_gameObject;
	}

	RigidBodyComponent& getBody() { return m_body; }
	bool& GetTrigger() { return m_isTrigger; }

private:
	RigidBodyComponent& m_body;
	bool m_isTrigger;
	sf::FloatRect m_AABB;
	sf::FloatRect m_transformedAABB;

	std::list<OnCollisionFunction> m_onCollisionFunctions;
};
