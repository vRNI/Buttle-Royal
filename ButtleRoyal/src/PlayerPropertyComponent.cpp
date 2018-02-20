#include "stdafx.h"
#include "PlayerPropertyComponent.h"
#include  "GameObject.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"

PlayerPropertyComponent::PlayerPropertyComponent(GameObject & gameObject, const int numberOfLives, const Vector2f sPos)
	: IComponent(gameObject), m_takenDamage(0), m_NormalGravity(true), m_lives(numberOfLives), m_startPosition(sPos),
	  m_onGround(false), m_hasJumped(false), m_hasDoubleJumped(false), m_hasHit(false)
{
}

void PlayerPropertyComponent::VUpdate(float fDeltaTime)
{
	if (m_onGround)
	{
		m_hasJumped = false;
		m_hasDoubleJumped = false;
		m_gameObject.GetComponent<RigidBodyComponent>()->changeIsOnGround(m_onGround);
	}

	if (InputManager::getInstance().isKeyPressed("punch", m_gameObject.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())
		|| InputManager::getInstance().isButtonPressed("punch", m_gameObject.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		m_hasHit = true;
	}
	else
	{
		m_hasHit = false;
	}

}

void PlayerPropertyComponent::changeGravity()
{
	m_gameObject.rotate(180.f);
	m_gameObject.scale(-1, 1);
	if (m_NormalGravity)
	{
		m_gameObject.move(0, 200);
	}
	else
	{
		m_gameObject.move(0, -200);
	}
	m_NormalGravity = !m_NormalGravity;
}
