#include "stdafx.h"

#include "PhysicsManager.h"

#include "BoxColliderComponent.h"
#include "RigidBodyComponent.h"

#include "PhysicsComponentEvents.h"

void PhysicsManager::Init()
{
	// subscribe to events to catch creation events of physics objects
	{
		EventBus::ListenerId id = EventBus::getInstance().AddListener(RigidBodyCreateEvent::sk_EventType,
			[this](IEvent::Ptr event)
		{
			auto rigidbodyEvent = std::static_pointer_cast<RigidBodyCreateEvent>(event);
			this->AddRigidBody(rigidbodyEvent->GetData());
		});
		m_listeners.push_back(id);
	}
	{
		EventBus::ListenerId id = EventBus::getInstance().AddListener(BoxColliderCreateEvent::sk_EventType,
			[this](IEvent::Ptr event)
		{
			auto colliderEvent = std::static_pointer_cast<BoxColliderCreateEvent>(event);
			this->AddCollider(colliderEvent->GetData());
		});
		m_listeners.push_back(id);
	}
}

void PhysicsManager::Shutdown()
{
	m_collider.clear();
	m_rigidBodies.clear();
	m_manifolds.clear();

	// unsubscribe from events
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
		EventBus::getInstance().RemoveListener(*it);
	m_listeners.clear();
}

void
PhysicsManager::AddCollider(BoxColliderComponent& collider)
{
	m_collider.push_back(&collider);
}

void 
PhysicsManager::RemoveCollider(BoxColliderComponent& collider)
{
	m_collider.remove(&collider);
}

void PhysicsManager::AddRigidBody(RigidBodyComponent& body)
{
	m_rigidBodies.push_back(&body);
}

void PhysicsManager::RemoveRigidBody(RigidBodyComponent& body)
{
	m_rigidBodies.remove(&body);
}

void
PhysicsManager::Update(float fDeltaTime)
{
	/*static float accumulator = 0.f;
	accumulator += fDeltaTime;
	float timestep = 1 / 200.f;

	while (accumulator > timestep)
	{
		accumulator -= timestep;*/
		// first reset manifolds
		m_manifolds.clear();

		// find collision and create manifold
		findCollisions();

		// now process collisions
		processCollisions();

		// integrate
		for (auto body : m_rigidBodies)
		{
			// apply velocity to bodies
			body->PhysicsUpdate(fDeltaTime);
		}
	//}
}

static Vector2f getCenter(const FloatRect& rect)
{
	return Vector2f(rect.left, rect.top) + 0.5f * Vector2f(rect.width, rect.height);
}

bool 
PhysicsManager::AABBvsAABB(const FloatRect& a, const FloatRect& b,
	Vector2f& normal, float& penetration)
{
	Vector2f n = getCenter(b) - getCenter(a);			// Vector from A to B
	float a_extent = a.width * 0.5f;			    // Calculate half extents along x axis
	float b_extent = b.width * 0.5f; 
	float x_overlap = a_extent + b_extent - abs(n.x);		// Calculate overlap on x axis

															// SAT test on x axis
	if (x_overlap > 0) {
		float a_extent = a.height * 0.5f;		// Calculate half extents along y axis
		float b_extent = b.height * 0.5f;
		float y_overlap = a_extent + b_extent - abs(n.y);	// Calculate overlap on y axis

															// SAT test on y axis
		if (y_overlap > 0) {
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap) {
				// Point towards B knowing that n points from A to B
				if (n.x < 0)
					normal = Vector2f(1.0f, 0.0f);
				else
					normal = Vector2f(-1.0f, 0.0f);
				penetration = x_overlap;
				return true;
			}
			else {
				// Point towards B knowing that n points from A to B
				if (n.y < 0)
					normal = Vector2f(0, 1);
				else
					normal = Vector2f(0, -1);
				penetration = y_overlap;
				return true;
			}
		}
	}
	return false;
}

void 
PhysicsManager::findCollisions()
{
	for (auto itA = m_collider.begin(); itA != m_collider.end(); ++itA)
	{
		BoxColliderComponent* colliderA = *itA;
		
		auto itB = itA;
		++itB;
		for (; itB != m_collider.end(); ++itB)
		{
			BoxColliderComponent* colliderB = *itB;

			if (colliderA->getBody().GetMass() == 0 && 
				colliderB->getBody().GetMass() == 0)
			{
				// if both object don't have a mass or body is the same skip
				continue;
			}

			//Player should not collide with each other
			if (colliderA->GetGameObject().GetId() == "PlayerOne" &&
				colliderB->GetGameObject().GetId() == "PlayerTwo")
				continue;
			
			if (colliderB->GetGameObject().GetId() == "PlayerOne" &&
				colliderA->GetGameObject().GetId() == "PlayerTwo")
				continue;
			

			Vector2f normal;
			float penetration;

			if (AABBvsAABB(colliderA->getTransformedAABB(), colliderB->getTransformedAABB(),
				normal, penetration))
			{
				
				colliderA->OnCollision(*colliderB);
				colliderB->OnCollision(*colliderA);

				if (colliderA->GetTrigger() || colliderB->GetTrigger())
				continue;

				Manifold manifold;
				manifold.colliderA = colliderA;
				manifold.colliderB = colliderB;
				manifold.normal = normal;
				manifold.penetration = penetration;

				m_manifolds.push_back(manifold);
			}
		}
	}
}

void 
PhysicsManager::processCollisions()
{
	for (Manifold manifold : m_manifolds)
	{
		//if (m_resolveCollision)
		{
			resolveCollision(&manifold.colliderA->getBody(), &manifold.colliderB->getBody(), 
				manifold.normal);
		}

		bool bPositionalCorrection = true;
		if (bPositionalCorrection)
		{
			positionalCorrection(&manifold.colliderA->getBody(), &manifold.colliderB->getBody(), 
				manifold.penetration, manifold.normal);
		}

		// Notify any subscribed components by calling the 
		// registered OnCollisionFunction function
		// TODO: careful! not tested!
		manifold.colliderA->OnCollision(*manifold.colliderB);
		manifold.colliderB->OnCollision(*manifold.colliderA);
	}
}

void 
PhysicsManager::resolveCollision(RigidBodyComponent* a, RigidBodyComponent* b,
	Vector2f& normal)
{
	// Calculate relative velocity
	Vector2f rv = a->GetVelocity() - b->GetVelocity();

	// Calculate relative velocity in terms of the normal direction
	float velAlongNormal = rv.x * normal.x + rv.y * normal.y;

	// Do not resolve if velocities are separating
	if (velAlongNormal > 0)
		return;

	bool restitutionOn = true;
	if (restitutionOn)
	{
		// Calculate impulse scalar
		const float e = 0.1f; //< 1.0 objects are very bouncy, 0.0 not bouncy
		float j = -(1 + e) * velAlongNormal;
		j /= a->GetInvMass() + b->GetInvMass();

		// Apply impulse
		Vector2f impulse = j * normal;

		a->AddVelocity( impulse * a->GetInvMass());
		b->AddVelocity(-impulse * b->GetInvMass());

		//err() << "velAlongNormal " << velAlongNormal << " impulse " << impulse.x << " " << impulse.y << endl;
	}
	else 
	{
		// Apply impulse
		Vector2f impulse = velAlongNormal * normal;

		// does not take into account mass
		a->AddVelocity(-0.5f * impulse);
		b->AddVelocity( 0.5f * impulse);
	}
}

void 
PhysicsManager::positionalCorrection(RigidBodyComponent* rigidBodyOne, RigidBodyComponent* rigidBodyTwo,
	float penetrationDepth, const Vector2f& normal)
{
	const float percent = 0.2f; // usually 20% to 80%
	const float slop = 0.01f; // usually 0.01 to 0.1
	Vector2f correction = max(penetrationDepth - slop, 0.0f) /
		(rigidBodyOne->GetInvMass() + rigidBodyTwo->GetInvMass()) * percent * normal;
	// Add to rigid body, so that it can be applied to position: pos += invMass * correction
	rigidBodyOne->AddPositionalCorrection( rigidBodyOne->GetInvMass() * correction);
	rigidBodyTwo->AddPositionalCorrection(-rigidBodyTwo->GetInvMass() * correction);
}

