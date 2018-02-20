
#include "stdafx.h"
#include "LeftLandingState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


LeftLandingState::LeftLandingState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftLandingState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_LANDINGLEFT);

	if (InputManager::getInstance().isKeyDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "LeftAirPunchState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x > 1.2f)
	{
		return "RightLandingState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y < -100.f)
	{
		return "LeftAirbornState";
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