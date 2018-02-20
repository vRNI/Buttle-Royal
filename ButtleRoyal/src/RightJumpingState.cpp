
#include "stdafx.h"
#include "RightJumpingState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


RightJumpingState::RightJumpingState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string RightJumpingState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_LANDINGRIGHT);
	player.GetComponent<PlayerPropertyComponent>()->setHasJumped(true);
	player.GetComponent<PlayerPropertyComponent>()->setOnGround(false);
	
	if (InputManager::getInstance().isKeyDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()) ||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "RightAirPunchState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x < -1.2f)
	{
		return "LeftJumpingState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > -300.f)
	{
		return "RightAirbornState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 100.f)
	{
		return "RightLandingState";
	}
	
	if(player.GetComponent<PlayerPropertyComponent>()->isOnGround())
	{
		if(player.GetComponent<RigidBodyComponent>()->GetVelocity().x < 1.2f)
		{
			return "RightStandingState";
		}
		return "RightRunningState";
	}

	
	return "";
}