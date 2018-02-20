#include "stdafx.h"

#include "GameObject.h"
#include "PlayerMoveComponent.h"
#include "InputManager.h"
#include "PlayerPropertyComponent.h"
#include "SoundManager.h"

Vector2f normalize(const Vector2f& input)
{
	const float length = sqrt(input.x * input.x + input.y * input.y);
	if (length != 0)
		return Vector2f(input.x / length, input.y / length);

	return input;
}

PlayerMoveComponent::PlayerMoveComponent(
	GameObject &gameObject, RigidBodyComponent& rigidBody, const int playerIndex)
	: IComponent(gameObject)
	, m_iPlayerIndex(playerIndex)
	, m_rigidBody(rigidBody)
{
}

bool PlayerMoveComponent::VInit()
{
	return true;
}

void PlayerMoveComponent::VUpdate(float fDeltaTime)
{
	auto playerProperties = m_gameObject.GetComponent<PlayerPropertyComponent>();

	if(playerProperties->isNormalGravity())
		m_rigidBody.AddVelocity(Vector2f(0, 15.f));
	else
		m_rigidBody.AddVelocity(Vector2f(0, -15.f));
	const float scale = 50;
	const float speed = 0.1f; // pixels/second 
	Vector2f translation{};
	const Vector2f controllerInput = InputManager::getInstance().getAxixPosition(m_iPlayerIndex);

	if (InputManager::getInstance().isKeyDown("right", m_iPlayerIndex))
		translation.x += speed * fDeltaTime;

	if (InputManager::getInstance().isKeyDown("left", m_iPlayerIndex))
		translation.x -= speed * fDeltaTime;

	if (InputManager::getInstance().isKeyPressed("jump", m_iPlayerIndex)
		|| InputManager::getInstance().isButtonPressed("jump", m_iPlayerIndex))
	{
		if(!playerProperties->hasDoubleJumped() ) // if there is still a jump left
		{
			if (!playerProperties->hasJumped()) // if player hasn´t jumped once
			{
				playerProperties->setHasJumped(true); // jumped once
			}
			else
			{
				playerProperties->setHasDoubleJumped(true); // jumped twice
			}

			m_rigidBody.SetVelocity(Vector2f(m_rigidBody.GetVelocity().x, 0)); // velocity to zero before setting new jumpvelocity
			SoundManager::getInstance().PlaySound("jump1");

			if (playerProperties->isNormalGravity())
			{
				m_rigidBody.AddVelocity(Vector2f(0, -900.f)); // for normal player
			}
			else
			{
				m_rigidBody.AddVelocity(Vector2f(0, 900.f)); // for upside-down palyer
			}
		}		
	}

	const Vector2f normalized = normalize(controllerInput);
	if(abs(controllerInput.x) < 0.01)
	{
		translation.x *= 60000;	// keyboard input scaling
	}
	else
	{		
		translation.x = normalized.x * scale; // controller input saling
	}


#if 1 // physics movement
	m_rigidBody.AddVelocity(translation);
#else // no physics movement
	m_gameObject.GetTransform().translate(translation);
#endif
}




