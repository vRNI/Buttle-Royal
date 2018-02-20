#pragma once

#include <list>

#include "EventBus.h"

class BoxColliderComponent;
class RigidBodyComponent;

class PhysicsManager
{
public:

	struct Manifold
	{
		Vector2f normal;
		float penetration;
		BoxColliderComponent* colliderA;
		BoxColliderComponent* colliderB;
	};

	void Init();
	void Shutdown();

	void AddCollider(BoxColliderComponent& collider);
	void RemoveCollider(BoxColliderComponent& collider);

	void AddRigidBody(RigidBodyComponent& body);
	void RemoveRigidBody(RigidBodyComponent& body);

	void Update(float fDeltaTime);

	std::list<RigidBodyComponent*> GetRigidBodies() { return m_rigidBodies; }
	std::list<BoxColliderComponent*> GetColliderComponents() { return m_collider; }
private:

	/// returns true if the 2 rectangles intersect
	/// \normal filled with the collision normal
	/// \penetration set with the collision penetration depth of the 
	/// two bodies along the collision normal.
	bool AABBvsAABB(const FloatRect& a, const FloatRect& b, 
		Vector2f& normal, float& penetration);

	/// Finds all collision and stores collision information to the manifold list.
	void findCollisions();

	/// Processes all collision in the manifold list
	void processCollisions();

	/// Resolves a collision of 2 bodies by adding an impulse to both objects
	void resolveCollision(RigidBodyComponent* a, RigidBodyComponent* b, 
		Vector2f& normal);

	/// Linear positional corrections of overlapping bodies.
	void positionalCorrection(RigidBodyComponent* a, RigidBodyComponent* b, 
		float penetrationDepth, const Vector2f& normal);

	std::list<RigidBodyComponent*> m_rigidBodies;
	std::list<BoxColliderComponent*> m_collider;
	std::list<Manifold> m_manifolds;

	// maintain a list of listeners that the manager is subscribed to,
	// so that he can unsubscribe
	std::list<EventBus::ListenerId> m_listeners;
};