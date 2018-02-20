
#include "stdafx.h"
#include "LeftAirbornState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


LeftAirbornState::LeftAirbornState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftAirbornState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_AIRBORNLEFT);

	if (InputManager::getInstance().isKeyDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "LeftAirPunchState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x > 1.2f)
	{
		return "RightAirbornState";
	}
	
	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 200.f)
	{
		return "LeftLandingState";
	}
	
	if(player.GetComponent<PlayerPropertyComponent>()->isOnGround())
	{
		if(abs(player.GetComponent<RigidBodyComponent>()->GetVelocity().x) < 1.2f)
		{
			return "LeftStandingState";
		}
		return "LeftRunningState";
	}

	return "";
}