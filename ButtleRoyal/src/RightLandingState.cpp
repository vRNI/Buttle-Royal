
#include "stdafx.h"
#include "RightLandingState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


RightLandingState::RightLandingState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string RightLandingState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_LANDINGRIGHT);

	if (InputManager::getInstance().isKeyDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()) ||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "RightAirPunchState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x < -1.2f)
	{
		return "LeftLandingState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y < -100.f)
	{
		return "RightAirbornState";
	}
	

	if(player.GetComponent<PlayerPropertyComponent>()->isOnGround())
	{
		if(abs(player.GetComponent<RigidBodyComponent>()->GetVelocity().x) < 1.2f)
		{
			return "RightStandingState";
		}
		return "RightRunningState";
	}
	
	return "";
}